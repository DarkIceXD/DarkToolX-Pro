#include "features.hpp"

static best_target get_best_hitbox_angle(player_t* entity, const int hp, const vec3_t& local_head, const vec3_t& aimpunch, const vec3_t& current_viewangle, const weapon_info_t* weaponData)
{
	constexpr int hitboxes[] = { hitbox_pelvis, hitbox_stomach, hitbox_lower_chest, hitbox_chest, hitbox_upper_chest, hitbox_head, hitbox_neck, hitbox_right_thigh, hitbox_left_thigh, hitbox_right_calf, hitbox_left_calf, hitbox_right_foot, hitbox_left_foot, hitbox_right_upper_arm, hitbox_right_forearm, hitbox_left_upper_arm, hitbox_left_forearm };
	best_target target = {};
	for (const auto i : hitboxes) {
		if (!csgo::conf->aimbot().is_hitbox_enabled(i))
			continue;
		auto current_hitbox = entity->get_hitbox_position(i);
		const auto new_viewangle = (math::calculate_angle(local_head, current_hitbox) - aimpunch).normalized_angles();
		if (math::fov(current_viewangle, new_viewangle) > csgo::conf->aimbot().fov)
			continue;
		const auto data = features::auto_wall(current_hitbox - local_head, weaponData, true);
		if (data.damage > target.damage)
		{
			target.entity = data.entity;
			target.damage = static_cast<int>(data.damage);
			target.angle = new_viewangle;
			target.lethal = target.damage >= hp;
			target.hitbox = i;
			if (target.lethal)
				break;
		}
	}
	return target;
}

static best_target get_best_target(const vec3_t& viewangles, const weapon_info_t* weapon_data)
{
	const auto local_head = csgo::local_player->get_eye_pos();
	const auto aimpunch = csgo::local_player->recoil();
	best_target target = {};
	for (auto i = 1; i <= interfaces::globals->max_clients; i++)
	{
		auto entity = static_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity || entity->dormant() || !entity->is_player() || entity == csgo::local_player)
			continue;

		const auto hp = entity->health();
		if (hp < 1)
			continue;

		if (entity->has_gun_game_immunity())
			continue;

		if (!csgo::local_player->is_enemy(entity))
			continue;

		const auto target_i = get_best_hitbox_angle(entity, hp, local_head, aimpunch, viewangles, weapon_data);
		if (target_i.lethal)
			return target_i;

		if (target_i.damage > target.damage)
			target = target_i;
	}
	return target;
}

static bool is_visible(player_t* entity, vec3_t vStart, vec3_t vEnd)
{
	matrix_t matrix[MAXSTUDIOBONES];

	entity->setup_bones(matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, interfaces::globals->cur_time);

	auto pStudioModel = interfaces::model_info->get_studio_model(entity->model());

	auto set = pStudioModel->hitbox_set(entity->hitbox_set());

	for (int i = 0; i < set->hitbox_count; i++)
	{
		auto hitbox = set->hitbox(i);

		auto bone = hitbox->bone;

		auto vMaxUntransformed = hitbox->maxs;
		auto vMinUntransformed = hitbox->mins;

		if (hitbox->radius != -1.f)
		{
			vMinUntransformed -= vec3_t(hitbox->radius, hitbox->radius, hitbox->radius);
			vMaxUntransformed += vec3_t(hitbox->radius, hitbox->radius, hitbox->radius);
		}

		if (math::IntersectLineWithOBB(vStart, vEnd, vMinUntransformed, vMaxUntransformed, matrix[hitbox->bone]))
			return true;
	}

	return false;
}

static bool auto_cock_revolver(const short index, weapon_t* weapon)
{
	if (index == WEAPON_REVOLVER && weapon->postpone_fire_ready() - 1.f >= interfaces::globals->cur_time)
		return true;
	return false;
}

void features::aimbot(c_usercmd* cmd)
{
	if (csgo::conf->aimbot().mode != 1)
		return;

	if (csgo::conf->aimbot().key_bind_type == 1 && GetAsyncKeyState(csgo::conf->aimbot().key_bind) & 1)
		csgo::conf->aimbot().enabled = !csgo::conf->aimbot().enabled;
	else if (csgo::conf->aimbot().key_bind_type == 2 && !GetAsyncKeyState(csgo::conf->aimbot().key_bind))
		return;

	if (csgo::conf->aimbot().min_dmg_override_key_bind_type == 1 && GetAsyncKeyState(csgo::conf->aimbot().min_dmg_override_key_bind) & 1)
		csgo::conf->aimbot().min_dmg_override_active = !csgo::conf->aimbot().min_dmg_override_active;
	else if (csgo::conf->aimbot().min_dmg_override_key_bind_type == 2)
		csgo::conf->aimbot().min_dmg_override_active = GetAsyncKeyState(csgo::conf->aimbot().min_dmg_override_key_bind);

	if (!csgo::conf->aimbot().enabled)
		return;

	if (!csgo::local_player)
		return;

	const auto time = interfaces::globals->interval_per_tick * cmd->tick_count;
	if (csgo::local_player->next_attack() > time)
		return;

	const auto weapon = csgo::local_player->active_weapon();
	if (!weapon)
		return;

	if (weapon->clip1_count() < 1)
		return;

	const auto weapon_data = weapon->get_weapon_data();
	if (!weapon_data)
		return;

	if (!weapon_data->weapon_full_auto && weapon->next_primary_attack() > time)
		return;

	const auto index = weapon->item_definition_index();
	if (csgo::conf->aimbot().auto_cock_revolver && auto_cock_revolver(index, weapon))
	{
		cmd->buttons |= in_attack;
		return;
	}

	csgo::target = get_best_target(cmd->viewangles, weapon_data);
	if (csgo::target.damage < 1)
		return;

	const bool is_shooting_manually = cmd->buttons & in_attack;
	if (is_shooting_manually)
	{
		cmd->viewangles = csgo::target.angle;
	}
	else if (csgo::conf->aimbot().auto_shoot)
	{
		const auto weapon_setting = csgo::conf->aimbot().get_weapon_settings(index);
		if (!(csgo::target.lethal || csgo::target.damage >= (csgo::conf->aimbot().min_dmg_override_active ? weapon_setting.min_dmg_override : weapon_setting.min_dmg)))
			return;

		if (csgo::conf->aimbot().auto_scope && weapon_data->weapon_type == WEAPONTYPE_SNIPER_RIFLE && !csgo::local_player->is_scoped())
			cmd->buttons |= in_attack2;

		if (!features::hitchance(csgo::target.angle, csgo::target.entity, weapon_setting.hitchance, weapon, weapon_data->weapon_range))
			return;

		cmd->viewangles = csgo::target.angle;
		cmd->buttons |= in_attack;
		csgo::want_to_shoot = true;
	}
}