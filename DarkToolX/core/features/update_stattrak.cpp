#include "features.hpp"
#include "../../dependencies/utilities/json_utils.h"

void features::events::update_stattrak(i_game_event* event)
{
	if (!csgo::cfg.skin_changer().enabled)
		return;

	if (!csgo::local_player)
		return;

	const auto local_player_index = csgo::local_player->index();
	if (interfaces::engine->get_player_for_user_id(event->get_int("attacker")) != local_player_index ||
		interfaces::engine->get_player_for_user_id(event->get_int("userid")) == local_player_index)
		return;

	const auto weapon = csgo::local_player->active_weapon();
	if (!weapon || weapon->item_id_high() != -1)
		return;

	const auto skin = csgo::cfg.skin_changer().get_skin(weapon->item_definition_index());
	if (!skin || !skin->count_kills)
		return;

	skin->stat_trak++;
	if (csgo::cfg.skin_changer().auto_save)
		json_utils::save(conf::file_name, json_utils::type::JSON, csgo::cfg);
	weapon->stat_trak() = skin->stat_trak;
	weapon->net_post_data_update(0);
}