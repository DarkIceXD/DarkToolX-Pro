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

	const auto ticks_to_choke = csgo::conf->misc().fake_lag ? csgo::conf->misc().fake_lag_ticks : (csgo::conf->misc().anti_aim >= 2 ? 1 : 0);
	send_packet = interfaces::clientstate->choked_commands >= ticks_to_choke;
}