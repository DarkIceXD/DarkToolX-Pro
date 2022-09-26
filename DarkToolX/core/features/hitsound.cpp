#include "features.hpp"

void features::events::hit_sound(i_game_event* event)
{
	if (!csgo::cfg.misc().hitsound)
		return;

	if (!csgo::local_player)
		return;

	const auto local_player_index = csgo::local_player->index();
	if (interfaces::engine->get_player_for_user_id(event->get_int("attacker")) != local_player_index ||
		interfaces::engine->get_player_for_user_id(event->get_int("userid")) == local_player_index)
		return;

	interfaces::engine->execute_cmd("play buttons/arena_switch_press_02");
}