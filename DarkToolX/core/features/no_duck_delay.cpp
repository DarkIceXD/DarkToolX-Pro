#include "features.hpp"

void features::no_duck_delay(c_usercmd* cmd)
{
	if (!csgo::cfg.misc().no_duck_delay)
		return;

	cmd->buttons |= in_bullrush;
}