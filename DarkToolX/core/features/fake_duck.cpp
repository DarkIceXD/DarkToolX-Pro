#include "features.hpp"

void features::fake_duck(c_usercmd* cmd, bool& send_packet)
{
	csgo::conf->misc().fake_duck.run();

	if (!csgo::conf->misc().fake_duck.enabled)
		return;

	if (!csgo::local_player)
		return;

	if (!csgo::local_player->is_alive())
		return;

	if (!interfaces::clientstate->net_channel)
		return;

	if (csgo::local_player->is_in_air())
		return;

	if (csgo::want_to_shoot)
	{
		if (csgo::local_player->duck_amount() != 0)
		{
			cmd->buttons &= ~in_attack;
			csgo::want_to_shoot = false;
		}
		else 
		{
			return;
		}
	}
		
	send_packet = interfaces::clientstate->choked_commands >= 14;
	if (interfaces::clientstate->choked_commands >= 7)
		cmd->buttons |= in_duck;
	else
		cmd->buttons &= ~in_duck;

	cmd->buttons |= in_bullrush;
}