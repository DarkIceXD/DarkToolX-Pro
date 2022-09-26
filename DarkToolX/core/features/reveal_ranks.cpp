#include "features.hpp"

void features::reveal_ranks(const c_usercmd* cmd)
{
	if (!csgo::cfg.misc().reveal_ranks)
		return;

	if (!(cmd->buttons & in_score))
		return;

	interfaces::client->dispatch_user_message(cs_um_serverrankrevealall, 0, 0);
}