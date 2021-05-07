#include "features.hpp"

void features::events::chat_bot(i_game_event* event)
{
	if (!csgo::conf->chat_bot().enabled)
		return;

	const auto size = csgo::conf->chat_bot().msgs.size();
	if (size < 1)
		return;

	const auto local_player_index = csgo::local_player->index();
	if (interfaces::engine->get_player_for_user_id(event->get_int("attacker")) != local_player_index ||
		interfaces::engine->get_player_for_user_id(event->get_int("userid")) == local_player_index)
		return;

	const auto rand = std::rand() % size;
	interfaces::engine->execute_cmd(("say " + csgo::conf->chat_bot().msgs.at(rand).msg).c_str());
}