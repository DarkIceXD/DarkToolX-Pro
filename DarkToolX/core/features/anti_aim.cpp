#include "features.hpp"

void features::anti_aim(c_usercmd* cmd)
{
	if (csgo::conf->misc().anti_aim != 1)
		return;

	if (!csgo::local_player)
		return;

	if (!csgo::local_player->is_alive() || csgo::local_player->flags() & fl_frozen)
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

	cmd->viewangles.x = 89;
	cmd->viewangles.y += 180;
}