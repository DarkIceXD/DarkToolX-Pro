#include "features.hpp"

void features::bunny_hop(c_usercmd* cmd, float& sidemove)
{
	if (!csgo::conf->misc().bhop)
		return;

	if (!csgo::local_player)
		return;

	const int move_type = csgo::local_player->move_type();
	if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer)
		return;

	if (!(csgo::local_player->flags() & fl_onground))
	{
		cmd->buttons &= ~in_jump;
		/*if (cmd->mousedx < 0)
			sidemove = -450.0f;
		else if (cmd->mousedx > 0)
			sidemove = 450.0f;*/
	}
}