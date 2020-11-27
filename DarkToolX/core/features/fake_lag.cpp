#include "features.hpp"

void features::fake_lag(bool& send_packet)
{
	if (!csgo::local_player->is_alive())
		return;

	auto pNetChannel = interfaces::clientstate->net_channel;
	if (pNetChannel == nullptr)
		return;

	const int iMaxCmdProcessTicks = csgo::conf->misc().fake_lag ? csgo::conf->misc().fake_lag_ticks : csgo::conf->misc().anti_aim ? 1 : 0;
	send_packet = interfaces::clientstate->choked_commands >= iMaxCmdProcessTicks;
}