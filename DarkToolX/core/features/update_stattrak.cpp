#include "features.hpp"

void features::events::update_stattrak(i_game_event* event)
{
	if (!csgo::conf->skin_changer().enabled)
		return;

	if (!csgo::local_player)
		return;

	player_info_t info;
	interfaces::engine->get_player_info(csgo::local_player->index(), &info);

	if (event->get_int("attacker") != info.userid || event->get_int("userid") == info.userid)
		return;

	const auto weapon = csgo::local_player->active_weapon();
	if (!weapon || weapon->item_id_high() != -1)
		return;

	const auto skin = csgo::conf->skin_changer().get_skin(weapon->item_definition_index());
	if (!skin || !skin->count_kills)
		return;

	skin->stat_trak++;
	if (csgo::conf->skin_changer().auto_save)
		csgo::conf->save();
	weapon->stat_trak() = skin->stat_trak;
	weapon->net_post_data_update(0);
}