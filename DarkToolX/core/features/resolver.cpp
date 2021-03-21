#include "features.hpp"

static std::array<int, 65> missed_shots;

void features::resolver::run()
{
	for (auto i = 1; i <= interfaces::globals->max_clients; i++)
	{
		auto entity = static_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity || entity->dormant() || !entity->is_player() || entity == csgo::local_player)
			continue;

		if (!entity->is_alive())
			continue;

		if (entity->has_gun_game_immunity())
			continue;

		if (!csgo::local_player->is_enemy(entity))
			continue;

		/*shots.entity = entity;
		shots.eye_angles = entity->eye_angles();*/
		const auto max_delta = entity->max_desync_angle();
		switch (missed_shots[i] % 5)
		{
		default:
			break;
		case 1:
			entity->eye_angles().y -= max_delta;
			break;
		case 2:
			entity->eye_angles().y += max_delta;
			break;
		case 3:
			entity->eye_angles().y -= max_delta / 2;
			break;
		case 4:
			entity->eye_angles().y += max_delta / 2;
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
	console::log("==============\n");
	console::log("shot fired: %d\n", csgo::target.entity->index());
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
	console::log("shot hit: %d\n", interfaces::engine->get_player_for_user_id(event->get_int("userid")));
	console::log("==============\n");
}