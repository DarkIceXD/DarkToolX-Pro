#include "features.hpp"

void features::slow_walk(c_usercmd* cmd, float& forwardmove, float& sidemove)
{
	if (!csgo::conf->misc().slow_walk)
		return;

	if (!(cmd->buttons & in_speed))
		return;

	cmd->buttons &= ~in_speed;

	if (!csgo::local_player)
		return;

	const auto weapon = csgo::local_player->active_weapon();
	if (!weapon)
		return;

	const auto weapon_data = weapon->get_weapon_data();
	if (!weapon_data)
		return;

	const auto max_speed = (csgo::local_player->is_scoped() ? weapon_data->weapon_max_speed_alt : weapon_data->weapon_max_speed) * 0.34f;
	if (csgo::local_player->velocity().length() > max_speed)
	{
		cmd->buttons &= ~in_walk;
		forwardmove = 0;
		sidemove = 0;
	}
}