#include "features.hpp"

void features::events::chat_bot(i_game_event* event)
{
	if (!csgo::conf->chat_bot().enabled)
		return;

	const auto size = csgo::conf->chat_bot().msgs.size();
	if (size < 1)
		return;

	player_info_t info;
	interfaces::engine->get_player_info(csgo::local_player->index(), &info);

	if (event->get_int("attacker") != info.userid || event->get_int("userid") == info.userid)
		return;

	const auto rand = std::rand() % size;
	interfaces::engine->execute_cmd(("say " + csgo::conf->chat_bot().msgs.at(rand).msg).c_str());
}

void features::events::chat_listener(i_game_event* event)
{

}