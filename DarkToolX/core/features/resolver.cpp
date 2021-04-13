#include "features.hpp"

static std::array<int, 65> missed_shots;

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

		const auto max_delta = entity->max_desync_angle();
		switch (missed_shots[i] % 5)
		{
		default:
			break;
		case 1:
			entity->lower_body_yaw() -= max_delta;
			break;
		case 2:
			entity->lower_body_yaw() += max_delta;
			break;
		case 3:
			entity->lower_body_yaw() -= max_delta / 2;
			break;
		case 4:
			entity->lower_body_yaw() += max_delta / 2;
			break;
		}
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
	if (event->get_int("attacker") != info.userid || event->get_int("userid") == info.userid)
		return;

	missed_shots[interfaces::engine->get_player_for_user_id(event->get_int("userid"))]--;
}