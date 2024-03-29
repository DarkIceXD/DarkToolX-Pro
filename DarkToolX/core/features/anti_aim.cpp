#include "features.hpp"

static bool updating_lby()
{
	static auto lby_time = 0.f;
	const auto anim_state = csgo::local_player->get_anim_state();
	if (!anim_state)
		return false;

	if (anim_state->velocity > 0.1f || std::abs(anim_state->jump_fall_velocity) > 100.0f)
	{
		lby_time = interfaces::globals->cur_time + 0.22f;
	}
	else if (interfaces::globals->cur_time > lby_time)
	{
		lby_time = interfaces::globals->cur_time + 1.1f;
		return true;
	}
	return false;
}

static void apply_desync(c_usercmd* cmd, bool& send_packet, const bool desync_left, const float max_desync_delta = FLT_MAX)
{
	if (updating_lby())
	{
		cmd->viewangles.y += desync_left ? -120 : 120;
		send_packet = false;
	}
	else if (!send_packet)
	{
		const auto desync_delta = (std::min)(std::abs(csgo::local_player->max_desync_angle()), max_desync_delta);
		cmd->viewangles.y += desync_left ? desync_delta : -desync_delta;
	}
}

static void apply_smart_anti_aim(vec3_t& view_angles)
{
	const auto local_head = csgo::local_player->get_eye_pos();
	auto best_fov = FLT_MAX;
	vec3_t best_angle = view_angles;
	for (auto i = 1; i <= interfaces::globals->max_clients; i++)
	{
		auto entity = static_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity || entity->dormant() || !entity->is_player() || entity == csgo::local_player)
			continue;

		if (!entity->is_alive())
			continue;

		if (!csgo::local_player->is_enemy(entity))
			continue;

		const auto new_viewangles = math::calculate_angle(local_head, entity->get_eye_pos()).normalized_angles();
		const auto fov = math::fov(view_angles, new_viewangles);
		if (best_fov > fov)
		{
			best_angle = new_viewangles;
			best_fov = fov;
		}
	}
	view_angles = best_angle;
}

static void apply_anti_aim(c_usercmd* cmd)
{
	switch (csgo::cfg.misc().anti_aim)
	{
	default:
		break;
	case 1:
		cmd->viewangles.x = 89;
		cmd->viewangles.y += 180;
		break;
	case 2:
		cmd->viewangles.x = static_cast<float>(csgo::cfg.misc().pitch);
		cmd->viewangles.y += csgo::cfg.misc().yaw;
		break;
	}
}

static bool desync_left(const int mode)
{
	static bool flip = false;
	switch (mode)
	{
	default:
		return true;
	case 2:
		return false;
	case 3:
		flip = !flip;
		return flip;
	}
}

void features::anti_aim(c_usercmd* cmd, bool& send_packet)
{
	if (!csgo::local_player)
		return;

	if (!csgo::local_player->is_alive())
		return;

	if (csgo::local_player->flags() & fl_frozen)
		return;

	const int move_type = csgo::local_player->move_type();
	if (move_type == movetype_ladder || move_type == movetype_noclip)
		return;

	if (cmd->buttons & in_use)
		return;

	const auto weapon = csgo::local_player->active_weapon();
	if (!weapon)
		return;

	if ((cmd->buttons & in_attack) && csgo::local_player->can_shoot())
	{
		const auto index = weapon->item_definition_index();
		if (index != WEAPON_REVOLVER)
			return;

		if (!features::util::cock_revolver(weapon))
			return;
	}

	const auto weapon_data = weapon->get_weapon_data();
	if (!weapon_data)
		return;

	const auto type = weapon_data->type;
	if (type == WEAPONTYPE_GRENADE)
	{
		if ((!weapon->is_pin_pulled() || (cmd->buttons & (in_attack | in_attack2))) && weapon->throw_time() > 0.f)
			return;
	}
	else if (type == WEAPONTYPE_KNIFE)
	{
		if (cmd->buttons & (in_attack | in_attack2))
			return;
	}

	if (csgo::cfg.misc().smart_anti_aim)
		apply_smart_anti_aim(cmd->viewangles);

	if (csgo::cfg.misc().anti_aim)
		apply_anti_aim(cmd);

	if (csgo::cfg.misc().desync)
		apply_desync(cmd, send_packet, desync_left(csgo::cfg.misc().desync), static_cast<float>(csgo::cfg.misc().max_desync_delta));
}