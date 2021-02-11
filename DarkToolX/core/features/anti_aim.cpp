#include "features.hpp"

void apply_desync(c_usercmd* cmd, bool& send_packet, const bool desync_left)
{
	const auto max_desync = std::abs(csgo::local_player->max_desync_angle());
	if (send_packet)
	{
		cmd->viewangles.y += desync_left ? max_desync : -max_desync;
	}
	else
	{
		cmd->viewangles.y += desync_left ? -120 : 120;
	}
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

	constexpr auto desync_left = true;
	switch (csgo::conf->misc().anti_aim)
	{
	default:
		cmd->viewangles.x = 89;
		cmd->viewangles.y += 180;
		break;
	case 2:
		apply_desync(cmd, send_packet, desync_left);
		break;
	case 3:
		cmd->viewangles.x = 89;
		cmd->viewangles.y += 180;
		apply_desync(cmd, send_packet, desync_left);
		break;
	}
}