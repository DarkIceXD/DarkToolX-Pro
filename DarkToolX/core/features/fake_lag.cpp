#include "features.hpp"

void features::fake_lag(bool& send_packet)
{
	if (!csgo::conf->misc().fake_lag)
		return;

	if (!csgo::local_player->is_alive())
		return;

	if (!interfaces::clientstate->net_channel)
		return;

	send_packet = interfaces::clientstate->choked_commands >= csgo::conf->misc().fake_lag_ticks;
}