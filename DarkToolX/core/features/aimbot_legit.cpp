#include "features.hpp"

static best_target get_best_hitbox_angle(player_t* entity, const int hp, const vec3_t& local_head, const vec3_t& aimpunch, const vec3_t& viewangles, const weapon_info_t* weapon_data)
{
	constexpr int hitboxes[] = { hitbox_pelvis, hitbox_stomach, hitbox_lower_chest, hitbox_chest, hitbox_upper_chest, hitbox_head, hitbox_neck, hitbox_right_thigh, hitbox_left_thigh, hitbox_right_calf, hitbox_left_calf, hitbox_right_foot, hitbox_left_foot, hitbox_right_upper_arm, hitbox_right_forearm, hitbox_left_upper_arm, hitbox_left_forearm };
	best_target target = {};
	target.entity = entity;
	target.fov = FLT_MAX;
	for (const auto hitbox : hitboxes) {
		if (!csgo::conf->aimbot().is_hitbox_enabled(hitbox))
			continue;
		const auto hitbox_position = entity->get_hitbox_position(hitbox);
		const auto new_viewangles = (math::calculate_angle(local_head, hitbox_position) /*- aimpunch*/).normalized_angles();
		const auto fov = math::fov(viewangles, new_viewangles);
		if (fov > csgo::conf->aimbot().fov)
			continue;
		const auto data = features::auto_wall(hitbox_position - local_head, weapon_data, false);
		if (data.entity && target.fov > fov)
		{
			target.damage = static_cast<int>(data.damage);
			target.angle = new_viewangles;
			target.fov = fov;
		}
	}

	return target;
}

static best_target get_best_hitbox_angle(player_t* entity, const int hp, const vec3_t& local_head, const vec3_t& aimpunch, const vec3_t& viewangles, const weapon_info_t* weapon_data, const std::deque<record>& records)
{
	auto target = get_best_hitbox_angle(entity, hp, local_head, aimpunch, viewangles, weapon_data);
	if (csgo::conf->aimbot().backtrack)
	{
		auto changed = false;
		for (size_t i = 3; i < records.size(); i++)
		{
			const auto& record = records[i];
			if (!features::backtrack::valid(record.simulation_time))
				continue;

			features::backtrack::restore_record(entity, i);
			changed = true;
			const auto target_i = get_best_hitbox_angle(entity, hp, local_head, aimpunch, viewangles, weapon_data);
			if (target_i.fov < target.fov)
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

static best_target get_best_target(const vec3_t& viewangles, const weapon_info_t* weapon_data)
{
	const auto local_head = csgo::local_player->get_eye_pos();
	const auto aimpunch = csgo::local_player->recoil();
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

		const auto target_i = get_best_hitbox_angle(entity, hp, local_head, aimpunch, viewangles, weapon_data, features::backtrack::get_records(i));
		if (target_i.fov < target.fov)
			target = target_i;
	}
	return target;
}

void features::aimbot::legit(c_usercmd* cmd, weapon_t* weapon, const weapon_info_t* weapon_data)
{
	csgo::target = get_best_target(cmd->viewangles, weapon_data);
	if (csgo::target.fov < FLT_MAX)
	{
		auto recoil_compensated_angle = (csgo::target.angle - csgo::local_player->recoil()).normalized_angles();
		if (csgo::conf->aimbot().smoothness > 1 && csgo::target.fov > 1)
		{
			const auto delta = (recoil_compensated_angle - cmd->viewangles).normalized_angles();
			recoil_compensated_angle = (cmd->viewangles + delta / csgo::conf->aimbot().smoothness).normalized_angles();
		}
		cmd->viewangles = recoil_compensated_angle;
		interfaces::engine->set_view_angles(recoil_compensated_angle);
	}
	const bool is_shooting_manually = cmd->buttons & in_attack;
	if (is_shooting_manually)
	{
		if (csgo::target.best_record)
			cmd->tick_count = features::backtrack::restore_tick_count(csgo::target.entity->index(), csgo::target.best_record);
	}
	else if (csgo::conf->aimbot().auto_shoot)
	{
		if (csgo::target.best_record)
			features::backtrack::restore_record(csgo::target.entity, csgo::target.best_record);
		auto_shoot(cmd, weapon, weapon_data);
		if (csgo::target.best_record)
		{
			if (cmd->buttons & in_attack)
				cmd->tick_count = features::backtrack::restore_tick_count(csgo::target.entity->index(), csgo::target.best_record);
			features::backtrack::restore_record(csgo::target.entity, 0);
		}
	}
}