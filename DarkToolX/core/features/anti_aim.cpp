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

	if (csgo::conf->misc().smart_anti_aim && csgo::target.entity)
		cmd->viewangles = csgo::target.angle;

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