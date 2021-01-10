#include "features.hpp"

struct best_target {
	player_t* entity;
	vec3_t angle;
	float fov;
};

static best_target get_best_hitbox_angle(player_t* entity, const int hp, vec3_t& local_head, const vec3_t& aimpunch, const vec3_t& current_viewangle, const weapon_info_t* weapon_data)
{
	constexpr int hitboxes[] = { hitbox_pelvis, hitbox_stomach, hitbox_lower_chest, hitbox_chest, hitbox_upper_chest, hitbox_head, hitbox_neck, hitbox_right_thigh, hitbox_left_thigh, hitbox_right_calf, hitbox_left_calf, hitbox_right_foot, hitbox_left_foot, hitbox_right_upper_arm, hitbox_right_forearm, hitbox_left_upper_arm, hitbox_left_forearm };
	best_target target = {};
	target.entity = entity;
	target.fov = FLT_MAX;
	for (const auto i : hitboxes) {
		if (!csgo::conf->aimbot().is_hitbox_enabled(i))
			continue;
		auto current_hitbox = entity->get_hitbox_position(i);
		const auto new_viewangle = (math::calculate_angle(local_head, current_hitbox) - aimpunch).normalized();
		const auto fov = math::fov(current_viewangle, new_viewangle);
		if (fov > csgo::conf->aimbot().fov)
			continue;
		const auto can_see = features::auto_wall(current_hitbox - local_head, weapon_data, false).entity != nullptr;
		if (can_see && target.fov > fov)
		{
			target.angle = new_viewangle;
			target.fov = fov;
		}
	}
	return target;
}

static best_target get_best_target(const vec3_t& viewangles, const weapon_info_t* weapon_data)
{
	auto local_head = csgo::local_player->get_eye_pos();
	const auto aimpunch = csgo::local_player->aim_punch_angle() * 2;
	best_target target = {};
	target.fov = FLT_MAX;
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
		if (target_i.fov < target.fov)
			target = target_i;
	}
	return target;
}

void features::legit_aimbot(c_usercmd* cmd)
{
	if (csgo::conf->aimbot().mode != 2)
		return;

	if (csgo::conf->aimbot().key_bind_type == 1 && GetAsyncKeyState(csgo::conf->aimbot().key_bind) & 1)
		csgo::conf->aimbot().enabled = !csgo::conf->aimbot().enabled;
	else if (csgo::conf->aimbot().key_bind_type == 2 && !GetAsyncKeyState(csgo::conf->aimbot().key_bind))
		return;

	if (!csgo::conf->aimbot().enabled)
		return;

	if (!csgo::local_player)
		return;

	const auto weapon = csgo::local_player->active_weapon();
	if (!weapon)
		return;

	if (weapon->clip1_count() < 1)
		return;

	const auto weapon_data = weapon->get_weapon_data();
	if (!weapon_data)
		return;

	auto target = get_best_target(cmd->viewangles, weapon_data);
	if (target.fov < FLT_MAX)
	{
		if (csgo::conf->aimbot().smoothness > 1 && target.fov > 1)
		{
			const auto delta = (target.angle - cmd->viewangles).normalized();
			target.angle = (cmd->viewangles + delta / csgo::conf->aimbot().smoothness).normalized();
		}
		cmd->viewangles = target.angle;
		interfaces::engine->set_view_angles(target.angle);
	}
	if (csgo::conf->aimbot().auto_shoot)
		auto_shoot(cmd, weapon, weapon_data);
}