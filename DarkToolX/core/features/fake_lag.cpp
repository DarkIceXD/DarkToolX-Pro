#include "features.hpp"

void features::fake_lag(bool& send_packet)
{
	if (csgo::want_to_shoot)
		return;

	if (!csgo::local_player)
		return;

	if (!csgo::local_player->is_alive())
		return;

	if (!interfaces::clientstate->net_channel)
		return;

	const auto ticks_to_choke = csgo::cfg.misc().fake_lag ? csgo::cfg.misc().fake_lag_ticks : (csgo::cfg.misc().desync ? 1 : 0);
	send_packet = interfaces::clientstate->choked_commands >= ticks_to_choke;
}