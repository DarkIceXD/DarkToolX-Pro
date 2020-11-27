#include "features.hpp"

void features::events::hit_sound(i_game_event* event)
{
	if (!csgo::conf->misc().hitsound)
		return;

	if (!csgo::local_player)
		return;

	player_info_t info;
	interfaces::engine->get_player_info(csgo::local_player->index(), &info);

	if (event->get_int("attacker") != info.userid || event->get_int("userid") == info.userid)
		return;

	interfaces::engine->execute_cmd("play buttons/arena_switch_press_02");
}