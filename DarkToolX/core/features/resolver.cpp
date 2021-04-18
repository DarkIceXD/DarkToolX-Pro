#include "features.hpp"

static std::array<int, 65> missed_shots;
static std::list<int> last_shots;
static bool new_shot = true;

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

void features::resolver::new_tick(c_usercmd* cmd)
{
	new_shot = true;
	if (csgo::target.entity && (cmd->buttons & in_attack))
		last_shots.push_back(csgo::target.entity->index());
}

void features::resolver::run()
{
	if (!csgo::conf->misc().resolver)
		return;

	for (auto i = 1; i <= interfaces::globals->max_clients; i++)
	{
		auto entity = static_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity || entity->dormant() || !entity->is_player() || entity == csgo::local_player || !entity->is_alive())
			continue;

		if (!csgo::local_player->is_enemy(entity))
			continue;

		const auto anim_state = entity->get_anim_state();
		if (!anim_state)
			continue;

		anim_state->goal_feet_yaw = entity->eye_angles().y + get_delta(entity->max_desync_angle(), missed_shots[i]);
	}
}

void features::resolver::weapon_fire(i_game_event* event)
{
	if (last_shots.empty())
		return;

	if (!csgo::local_player)
		return;

	player_info_t info;
	interfaces::engine->get_player_info(csgo::local_player->index(), &info);
	if (event->get_int("userid") != info.userid)
		return;

	missed_shots[last_shots.front()]++;
	last_shots.pop_front();
}

void features::resolver::bullet_impact(i_game_event* event)
{
	if (!csgo::target.entity)
		return;

	if (!csgo::local_player)
		return;

	if (!new_shot)
		return;

	player_info_t info;
	interfaces::engine->get_player_info(csgo::local_player->index(), &info);
	if (event->get_int("userid") != info.userid)
		return;


	new_shot = false;
}

void features::resolver::player_hurt(i_game_event* event)
{
	if (!csgo::local_player)
		return;

	player_info_t info;
	interfaces::engine->get_player_info(csgo::local_player->index(), &info);
	const auto victim = event->get_int("userid");
	if (event->get_int("attacker") != info.userid || victim == info.userid)
		return;

	missed_shots[interfaces::engine->get_player_for_user_id(victim)]--;
}