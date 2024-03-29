#include "features.hpp"

void features::slow_walk(c_usercmd* cmd)
{
	if (!csgo::cfg.misc().slow_walk)
		return;

	if (!(cmd->buttons & in_speed))
		return;
	
	if (cmd->forwardmove == 0 && cmd->sidemove == 0)
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
	
	const auto max_speed = (!csgo::local_player->is_scoped() ? weapon_data->max_speed : weapon_data->max_speed_alt) * 0.34f;
	const auto scale = max_speed / sqrt(cmd->forwardmove * cmd->forwardmove + cmd->sidemove * cmd->sidemove);
	cmd->forwardmove *= scale;
	cmd->sidemove *= scale;
}