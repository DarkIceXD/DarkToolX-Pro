#include "features.hpp"

void features::slow_walk(c_usercmd* cmd, float& forwardmove, float& sidemove)
{
	if (!csgo::conf->misc().slow_walk)
		return;

	if (!(cmd->buttons & in_speed))
		return;

	if (!csgo::local_player)
		return;

	if (!(csgo::local_player->flags() & fl_onground))
		return;

	const auto weapon = csgo::local_player->active_weapon();
	if (!weapon)
		return;

	const auto weapon_data = weapon->get_weapon_data();
	if (!weapon_data)
		return;

	const auto max_speed = (!csgo::local_player->is_scoped() ? weapon_data->weapon_max_speed : weapon_data->weapon_max_speed_alt) * 0.34f;
	const auto scale = max_speed / sqrt(forwardmove * forwardmove + sidemove * sidemove);
	forwardmove *= scale;
	sidemove *= scale;
}