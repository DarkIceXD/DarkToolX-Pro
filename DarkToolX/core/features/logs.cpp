#include "features.hpp"

constexpr auto darktoolx = "[""\x03""DarkToolX\x01]\x01 ";

struct player_event {
	int id;
	player_t* entity;
	bool is_enemy;
	constexpr char team_color() const
	{
		constexpr auto team_color = '\x0C';
		constexpr auto enemy_color = '\x07';
		return is_enemy ? enemy_color : team_color;
	}
};

static player_event get_player(const int user_id)
{
	player_event p;
	p.id = interfaces::engine->get_player_for_user_id(user_id);
	p.entity = static_cast<player_t*>(interfaces::entity_list->get_client_entity(p.id));
	p.is_enemy = csgo::local_player->is_enemy(p.entity);
	return p;
}

void features::logs::player_hurt(i_game_event* event)
{
	const auto& log = csgo::cfg.logs().damage;
	if (!log.enabled)
		return;

	if (!csgo::local_player)
		return;

	const auto chat = interfaces::clientmode->get_hud_chat();
	if (!chat)
		return;

	const auto attacker = get_player(event->get_int("attacker"));
	if (!log.is_enabled(!attacker.is_enemy, attacker.entity == csgo::local_player))
		return;

	const auto victim = get_player(event->get_int("userid"));
	player_info_t attacker_info, victim_info;
	interfaces::engine->get_player_info(attacker.id, &attacker_info);
	interfaces::engine->get_player_info(victim.id, &victim_info);
	chat->printf(0, "%s%c%s\x01 hit %c%s\x01 for %d hp (%d hp left)", darktoolx, attacker.team_color(), attacker.id ? attacker_info.name : "\x06World", victim.team_color(), victim_info.name, event->get_int("dmg_health"), event->get_int("health"));
}

void features::logs::player_death(i_game_event* event)
{
	const auto& log = csgo::cfg.logs().deaths;
	if (!log.enabled)
		return;

	if (!csgo::local_player)
		return;

	const auto chat = interfaces::clientmode->get_hud_chat();
	if (!chat)
		return;

	const auto attacker = get_player(event->get_int("attacker"));
	if (!log.is_enabled(!attacker.is_enemy, attacker.entity == csgo::local_player))
		return;

	const auto victim = get_player(event->get_int("userid"));
	player_info_t attacker_info, victim_info;
	interfaces::engine->get_player_info(attacker.id, &attacker_info);
	interfaces::engine->get_player_info(victim.id, &victim_info);
	chat->printf(0, "%s%c%s\x01 killed %c%s", darktoolx, attacker.team_color(), attacker.id ? attacker_info.name : "\x06World", victim.team_color(), victim_info.name);
}

void features::logs::vote_cast(i_game_event* event)
{
	if (!csgo::cfg.logs().votes)
		return;

	if (!csgo::local_player)
		return;

	const auto chat = interfaces::clientmode->get_hud_chat();
	if (!chat)
		return;

	player_event voter;
	voter.id = event->get_int("entityid");
	voter.entity = static_cast<player_t*>(interfaces::entity_list->get_client_entity(voter.id));
	voter.is_enemy = csgo::local_player->is_enemy(voter.entity);
	player_info_t voter_info;
	interfaces::engine->get_player_info(voter.id, &voter_info);
	const auto voted_yes = event->get_int("vote_option") == 0;
	chat->printf(0, "%s%c%s\x01 voted %s", darktoolx, voter.team_color(), voter_info.name, voted_yes ? "\x04yes" : "\x02no");
}

void features::logs::item_pickup(i_game_event* event)
{
	const auto& log = csgo::cfg.logs().pickup;
	if (!log.enabled)
		return;

	if (!csgo::local_player)
		return;
	
	const auto chat = interfaces::clientmode->get_hud_chat();
	if (!chat)
		return;

	const auto pickuper = get_player(event->get_int("userid"));
	if (!log.is_enabled(!pickuper.is_enemy, pickuper.entity == csgo::local_player))
		return;

	player_info_t pickuper_info;
	interfaces::engine->get_player_info(pickuper.id, &pickuper_info);
	chat->printf(0, "%s%c%s\x01 picked up %s", darktoolx, pickuper.team_color(), pickuper_info.name, weapon_t::get_weapon_name(event->get_int("defindex")));
}