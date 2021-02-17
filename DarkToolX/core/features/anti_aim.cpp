#include "features.hpp"

static bool updating_lby()
{
	static auto lby_time = 0.f;
	const auto anim_state = csgo::local_player->get_anim_state();
	if (!anim_state)
		return false;

	const auto curtime = csgo::local_player->get_tick_base() * interfaces::globals->interval_per_tick;
	if (anim_state->velocity > 0.1f || std::abs(anim_state->jump_fall_velocity) > 100.0f)
	{
		lby_time = curtime + 0.22f;
	}
	else if (curtime > lby_time)
	{
		lby_time = curtime + 1.1f;
		return true;
	}
	return false;
}

static void apply_desync(c_usercmd* cmd, bool& send_packet)
{
	const auto desync_left = cmd->sidemove < 0;
	const auto max_desync = std::abs(csgo::local_player->max_desync_angle());
	if (updating_lby())
	{
		cmd->viewangles.y += desync_left ? -120 : 120;
		send_packet = false;
	}
	else if (!send_packet)
	{
		cmd->viewangles.y += desync_left ? max_desync : -max_desync;
	}
	/*if (!send_packet)
		cmd->viewangles.y += desync_left ? max_desync : -max_desync;*/
}

static vec3_t get_best_angle(const vec3_t& view_angles)
{
	const auto local_head = csgo::local_player->get_eye_pos();
	auto valid = false;
	auto length = FLT_MAX;
	vec3_t best_eyes;
	for (auto i = 1; i <= interfaces::globals->max_clients; i++)
	{
		auto entity = static_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity || entity->dormant() || !entity->is_player() || entity == csgo::local_player)
			continue;

		const auto hp = entity->health();
		if (hp < 1)
			continue;

		if (!csgo::local_player->is_enemy(entity))
			continue;

		const auto& entity_eyes = entity->get_eye_pos();
		const auto new_length = (local_head - entity_eyes).length();
		if (new_length < length)
		{
			length = new_length;
			best_eyes = entity_eyes;
			valid = true;
		}
	}
	return valid ? math::calculate_angle(local_head, best_eyes) : view_angles;
}

void features::anti_aim(c_usercmd* cmd, bool& send_packet)
{
	if (!csgo::conf->misc().anti_aim)
		return;

	if (!csgo::local_player)
		return;

	if (!csgo::local_player->is_alive())
		return;

	if (csgo::local_player->flags() & fl_frozen)
		return;

	const int move_type = csgo::local_player->move_type();
	if (move_type == movetype_ladder || move_type == movetype_noclip)
		return;

	if ((cmd->buttons & in_use) || csgo::want_to_shoot)
		return;

	auto weapon = csgo::local_player->active_weapon();
	if (!weapon)
		return;

	const auto weapon_data = weapon->get_weapon_data();
	if (!weapon_data)
		return;

	const auto type = weapon_data->weapon_type;
	if (type == WEAPONTYPE_GRENADE)
		return;

	if (cmd->buttons & in_attack2 && type == WEAPONTYPE_KNIFE)
		return;

	if (csgo::conf->misc().smart_anti_aim)
		cmd->viewangles = csgo::target.entity ? csgo::target.angle : get_best_angle(cmd->viewangles);

	switch (csgo::conf->misc().anti_aim)
	{
	default:
		cmd->viewangles.x = 89;
		cmd->viewangles.y += 180;
		break;
	case 2:
		apply_desync(cmd, send_packet);
		break;
	case 3:
		cmd->viewangles.x = 89;
		cmd->viewangles.y += 180;
		apply_desync(cmd, send_packet);
		break;
	}
}