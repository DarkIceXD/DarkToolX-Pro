#include "features.hpp"

static std::array<int, 65> missed_shots;
static bool accept_shot = true;
static bool hit_player = false;

constexpr float get_delta(const float max_delta, const int misses)
{
	switch (misses % 5)
	{
	default:
		return 0;
	case 1:
		return -max_delta;
	case 2:
		return max_delta;
	case 3:
		return -max_delta / 2;
	case 4:
		return max_delta / 2;
	}
}

void features::resolver::new_tick()
{
	accept_shot = true;
	hit_player = false;
}

void features::resolver::run()
{
	if (!csgo::conf->misc().resolver)
	{
		missed_shots = {};
		return;
	}

	for (auto i = 1; i <= interfaces::globals->max_clients; i++)
	{
		auto entity = static_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity || entity->dormant() || !entity->is_player() || entity == csgo::local_player)
			continue;

		if (!entity->is_alive())
			continue;

		if (!csgo::local_player->is_enemy(entity))
			continue;

		const auto new_lby = entity->lower_body_yaw() + get_delta(entity->max_desync_angle(), missed_shots[i]);
		entity->lower_body_yaw() = std::isfinite(new_lby) ? std::remainder(new_lby, 360.0f) : 0.0f;
	}
}

void features::resolver::weapon_fire(i_game_event* event)
{
	if (!csgo::target.entity)
		return;

	if (!csgo::local_player)
		return;

	player_info_t info;
	interfaces::engine->get_player_info(csgo::local_player->index(), &info);
	if (event->get_int("userid") != info.userid)
		return;

	missed_shots[csgo::target.entity->index()]++;
}

void features::resolver::player_hurt(i_game_event* event)
{
	if (!csgo::target.entity)
		return;

	if (!csgo::local_player)
		return;

	player_info_t info;
	interfaces::engine->get_player_info(csgo::local_player->index(), &info);
	const auto victim = event->get_int("userid");
	if (event->get_int("attacker") != info.userid || victim == info.userid)
		return;

	missed_shots[interfaces::engine->get_player_for_user_id(victim)]--;
	hit_player = true;
}