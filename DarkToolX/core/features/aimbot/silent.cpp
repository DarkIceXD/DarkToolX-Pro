#include "../features.hpp"

static best_target get_best_hitbox_angle(player_t* entity, const int hp, const vec3_t& local_head, const vec3_t& viewangles, const weapon_info_t* weapon_data)
{
	constexpr int hitboxes[] = { hitbox_stomach, hitbox_lower_chest, hitbox_chest, hitbox_upper_chest, hitbox_head, hitbox_right_thigh, hitbox_left_thigh, hitbox_right_calf, hitbox_left_calf, hitbox_right_foot, hitbox_left_foot, hitbox_right_upper_arm, hitbox_right_forearm, hitbox_left_upper_arm, hitbox_left_forearm };
	best_target target = {};
	for (const auto hitbox : hitboxes) {
		if (!csgo::cfg.aimbot().is_hitbox_enabled(hitbox))
			continue;
		const auto hitbox_position = entity->get_hitbox_position(hitbox);
		const auto new_viewangles = math::calculate_angle(local_head, hitbox_position).normalized_angles();
		if (math::fov(viewangles, new_viewangles) > csgo::cfg.aimbot().fov)
			continue;
		const auto data = features::util::auto_wall(local_head, hitbox_position - local_head, weapon_data, csgo::cfg.aimbot().auto_wall);
		if (data.damage > target.damage)
		{
			target.entity = data.entity;
			target.damage = static_cast<int>(data.damage);
			target.angle = new_viewangles;
			target.aim_spot = hitbox_position;
			target.lethal = target.damage >= hp;
			if (target.lethal)
				break;
		}
	}
	return target;
}

static best_target get_best_hitbox_angle(player_t* entity, const int hp, const vec3_t& local_head, const vec3_t& viewangles, const weapon_info_t* weapon_data, const std::deque<record>& records)
{
	auto target = get_best_hitbox_angle(entity, hp, local_head, viewangles, weapon_data);
	if (csgo::cfg.aimbot().backtrack && !target.lethal)
	{
		auto changed = false;
		for (size_t i = 3; i < records.size(); i++)
		{
			const auto& record = records[i];
			if (!features::backtrack::valid(record.simulation_time))
				continue;

			features::backtrack::restore_record(entity, i);
			changed = true;
			const auto target_i = get_best_hitbox_angle(entity, hp, local_head, viewangles, weapon_data);
			if (target_i.lethal)
			{
				target = target_i;
				target.best_record = i;
				break;
			}

			if (target_i.damage > target.damage)
			{
				target = target_i;
				target.best_record = i;
			}
		}
		if (changed)
			features::backtrack::restore_record(entity, 0);
	}
	return target;
}

static best_target get_best_target(const vec3_t& viewangles, const weapon_info_t* weapon_data, const vec3_t& local_head)
{
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

		const auto target_i = get_best_hitbox_angle(entity, hp, local_head, viewangles, weapon_data, features::backtrack::get_records(i));
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
	const auto pStudioModel = interfaces::model_info->get_studio_model(entity->model());
	const auto set = pStudioModel->hitbox_set(entity->hitbox_set());
	for (int i = 0; i < set->hitbox_count; i++)
	{
		const auto hitbox = set->hitbox(i);
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

void features::aimbot::silent(c_usercmd* cmd, weapon_t* weapon, const weapon_info_t* weapon_data)
{
	csgo::target = get_best_target(cmd->viewangles, weapon_data, csgo::local_player->get_eye_pos());
	if (csgo::target.damage < 1)
	{
		const auto duckamount = csgo::local_player->duck_amount();
		if (duckamount > 0 && csgo::cfg.aimbot().auto_duck_peek)
		{
			csgo::target = get_best_target(cmd->viewangles, weapon_data, csgo::local_player->get_eye_pos() + vec3_t(0, 0, 18 * duckamount));
			const auto weapon_setting = csgo::cfg.aimbot().get_weapon_settings(weapon->item_definition_index());
			if (csgo::target.lethal || csgo::target.damage >= weapon_setting.get_min_dmg(csgo::cfg.aimbot().min_dmg_override.enabled))
				cmd->buttons &= ~in_duck;
		}
		return;
	}

	const auto recoil_compensated_angle = (csgo::target.angle - csgo::local_player->recoil()).normalized_angles();
	const bool is_shooting_manually = cmd->buttons & in_attack;
	if (is_shooting_manually)
	{
		if (csgo::target.best_record)
			cmd->tick_count = features::backtrack::restore_tick_count(csgo::target.entity->index(), csgo::target.best_record);
		cmd->viewangles = recoil_compensated_angle;
	}
	else if (csgo::cfg.aimbot().auto_shoot)
	{
		const auto weapon_setting = csgo::cfg.aimbot().get_weapon_settings(weapon->item_definition_index());
		if (!(csgo::target.lethal || csgo::target.damage >= weapon_setting.get_min_dmg(csgo::cfg.aimbot().min_dmg_override.enabled)))
		{
			features::util::walkbot(cmd);
			return;
		}

		if (csgo::target.best_record)
			features::backtrack::restore_record(csgo::target.entity, csgo::target.best_record);

		if (features::util::hitchance(recoil_compensated_angle, csgo::target.entity, weapon_setting.hitchance, weapon, weapon_data->range))
		{
			if (csgo::target.best_record)
				cmd->tick_count = features::backtrack::restore_tick_count(csgo::target.entity->index(), csgo::target.best_record);
			cmd->viewangles = recoil_compensated_angle;
			cmd->buttons |= in_attack;
			csgo::want_to_shoot = true;
		}
		else
		{
			if (csgo::cfg.aimbot().auto_scope && weapon_data->type == WEAPONTYPE_SNIPER_RIFLE && !csgo::local_player->is_scoped())
				cmd->buttons |= in_attack2;

			csgo::didnt_shoot_due_to_hitchance = true;
			if (csgo::target.best_record)
				features::backtrack::restore_record(csgo::target.entity, 0);
		}
	}
}