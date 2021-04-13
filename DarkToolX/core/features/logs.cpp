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

	if (!interfaces::clientmode->get_hud_chat())
		return;

	const auto attacker_id = interfaces::engine->get_player_for_user_id(event->get_int("attacker"));
	const auto attacker = static_cast<player_t*>(interfaces::entity_list->get_client_entity(attacker_id));
	const auto is_attacker_team = !csgo::local_player->is_enemy(attacker);
	if (!is_enabled(log, is_attacker_team, attacker == csgo::local_player))
		return;

	const auto victim_id = interfaces::engine->get_player_for_user_id(event->get_int("userid"));
	const auto victim = static_cast<player_t*>(interfaces::entity_list->get_client_entity(victim_id));
	const auto is_victim_team = !csgo::local_player->is_enemy(victim);
	player_info_t attacker_info, victim_info;
	interfaces::engine->get_player_info(attacker_id, &attacker_info);
	interfaces::engine->get_player_info(victim_id, &victim_info);
	interfaces::clientmode->get_hud_chat()->printf(0, "%s%c%s\x01 hit %c%s\x01 for %d", darktoolx, team(is_attacker_team), attacker_id ? attacker_info.name : "\x06World", team(is_victim_team), victim_info.name, event->get_int("dmg_health"));
}

void features::logs::player_death(i_game_event* event)
{
	const auto& log = csgo::conf->logs().deaths;
	if (!log.enabled)
		return;

	if (!interfaces::clientmode->get_hud_chat())
		return;

	const auto attacker_id = interfaces::engine->get_player_for_user_id(event->get_int("attacker"));
	const auto attacker = static_cast<player_t*>(interfaces::entity_list->get_client_entity(attacker_id));
	const auto is_attacker_team = !csgo::local_player->is_enemy(attacker);
	if (!is_enabled(log, is_attacker_team, attacker == csgo::local_player))
		return;

	const auto victim_id = interfaces::engine->get_player_for_user_id(event->get_int("userid"));
	const auto victim = static_cast<player_t*>(interfaces::entity_list->get_client_entity(victim_id));
	const auto is_victim_team = !csgo::local_player->is_enemy(victim);
	player_info_t attacker_info, victim_info;
	interfaces::engine->get_player_info(attacker_id, &attacker_info);
	interfaces::engine->get_player_info(victim_id, &victim_info);
	interfaces::clientmode->get_hud_chat()->printf(0, "%s%c%s\x01 killed %c%s", darktoolx, team(is_attacker_team), attacker_id ? attacker_info.name : "\x06World", team(is_victim_team), victim_info.name);
}

void features::logs::start_vote(i_game_event* event)
{
	if (!csgo::conf->logs().votes)
		return;

	if (!interfaces::clientmode->get_hud_chat())
		return;

	const auto vote_starter_id = interfaces::engine->get_player_for_user_id(event->get_int("userid"));
	const auto vote_starter = static_cast<player_t*>(interfaces::entity_list->get_client_entity(vote_starter_id));
	const auto is_vote_starter_team = !csgo::local_player->is_enemy(vote_starter);
	player_info_t vote_starter_info;
	interfaces::engine->get_player_info(vote_starter_id, &vote_starter_info);
	const auto type = event->get_int("type");
	if (type)
	{
		const auto victim_id = interfaces::engine->get_player_for_user_id(event->get_int("vote_parameter"));
		const auto victim = static_cast<player_t*>(interfaces::entity_list->get_client_entity(victim_id));
		const auto is_victim_team = !csgo::local_player->is_enemy(victim);
		player_info_t victim_info;
		interfaces::engine->get_player_info(victim_id, &victim_info);
		interfaces::clientmode->get_hud_chat()->printf(0, "%s%c%s\x01 started a votekick against %c%s", darktoolx, team(is_vote_starter_team), vote_starter_info.name, team(is_victim_team), victim_info.name);
	}
	else
	{
		interfaces::clientmode->get_hud_chat()->printf(0, "%s%c%s\x01 started a vote to surrender", darktoolx, team(is_vote_starter_team), vote_starter_info.name);
	}
}

void features::logs::vote_cast(i_game_event* event)
{
	if (!csgo::conf->logs().votes)
		return;

	if (!interfaces::clientmode->get_hud_chat())
		return;

	const auto voter_id = interfaces::engine->get_player_for_user_id(event->get_int("entityid"));
	const auto voter = static_cast<player_t*>(interfaces::entity_list->get_client_entity(voter_id));
	const auto is_voter_team = !csgo::local_player->is_enemy(voter);
	player_info_t voter_info;
	interfaces::engine->get_player_info(voter_id, &voter_info);
	const auto voted_yes = event->get_int("vote_option") == 0;
	interfaces::clientmode->get_hud_chat()->printf(0, "%s%c%s\x01 voted %s", darktoolx, team(is_voter_team), voter_info.name, voted_yes ? "\x04yes" : "\x02no");
}