#include "features.hpp"

constexpr auto darktoolx = "[""\x03""DarkToolX\x01]\x01 ";
constexpr char team(const bool is_team)
{
	constexpr auto team_color = '\x0C';
	constexpr auto enemy_color = '\x07';
	return is_team ? team_color : enemy_color;
}

constexpr bool is_enabled(const config::log& log, const bool team, const bool is_local_player)
{
	return team ? (is_local_player ? log.local_player : log.team) : log.enemy;
}

void features::logs::player_hurt(i_game_event* event)
{
	const auto& log = csgo::conf->logs().damage;
	if (!log.enabled)
		return;

	const auto attacker_id = interfaces::engine->get_player_for_user_id(event->get_int("attacker"));
	const auto attacker = static_cast<player_t*>(interfaces::entity_list->get_client_entity(attacker_id));
	const auto is_attacker_team = !csgo::local_player->is_enemy(attacker);
	const auto victim_id = interfaces::engine->get_player_for_user_id(event->get_int("userid"));
	const auto victim = static_cast<player_t*>(interfaces::entity_list->get_client_entity(victim_id));
	const auto is_victim_team = !csgo::local_player->is_enemy(victim);
	if (!is_enabled(log, is_attacker_team, attacker == csgo::local_player) && !is_enabled(log, is_victim_team, victim == csgo::local_player))
		return;

	player_info_t attacker_info, victim_info;
	interfaces::engine->get_player_info(attacker_id, &attacker_info);
	interfaces::engine->get_player_info(victim_id, &victim_info);
	const short dmg_done = event->get_int("dmg_health");
	interfaces::clientmode->get_hud_chat()->printf(0, "%s%c%s\x01 damaged %c%s\x01 for %d hp", darktoolx, team(is_attacker_team), attacker_info.name, team(is_victim_team), victim_info.name, dmg_done);
}

void features::logs::player_death(i_game_event* event)
{
	const auto& log = csgo::conf->logs().deaths;
	if (!log.enabled)
		return;

	const auto attacker_id = interfaces::engine->get_player_for_user_id(event->get_int("attacker"));
	const auto attacker = static_cast<player_t*>(interfaces::entity_list->get_client_entity(attacker_id));
	const auto is_attacker_team = !csgo::local_player->is_enemy(attacker);
	const auto victim_id = interfaces::engine->get_player_for_user_id(event->get_int("userid"));
	const auto victim = static_cast<player_t*>(interfaces::entity_list->get_client_entity(victim_id));
	const auto is_victim_team = !csgo::local_player->is_enemy(victim);
	if (!is_enabled(log, is_attacker_team, attacker == csgo::local_player) && !is_enabled(log, is_victim_team, victim == csgo::local_player))
		return;

	player_info_t attacker_info, victim_info;
	interfaces::engine->get_player_info(attacker_id, &attacker_info);
	interfaces::engine->get_player_info(victim_id, &victim_info);
	const short dmg_done = event->get_int("dmg_health");
	interfaces::clientmode->get_hud_chat()->printf(0, "%s%c%s\x01 killed %c%s", darktoolx, team(is_attacker_team), attacker_info.name, team(is_victim_team), victim_info.name);
}