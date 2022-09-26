#include "../features.hpp"
#include "../../../dependencies/utilities/navmesh-parser/nav_file.h"

player_t* get_nearest_player()
{
	const auto origin = csgo::local_player->origin();
	float best_distance = FLT_MAX;
	player_t* best_player = nullptr;
	for (auto i = 1; i <= interfaces::globals->max_clients; i++)
	{
		auto entity = static_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity || entity->dormant() || !entity->is_player() || entity == csgo::local_player)
			continue;

		if (!entity->is_alive())
			continue;

		if (!csgo::local_player->is_enemy(entity))
			continue;

		const auto distance = (origin - entity->origin()).length();
		if (best_distance > distance)
		{
			best_distance = distance;
			best_player = entity;
		}
	}
	return best_player;
}

static std::string map;
static size_t i = 0;
static std::vector<vec3_t> path;
static nav_mesh::nav_file nav;
static player_t* target = nullptr;
static bool valid = false;

void features::util::walkbot(c_usercmd* cmd)
{
	if (!csgo::cfg.misc().walk_bot)
	{
		map.clear();
		path.clear();
		return;
	}

	if (!csgo::local_player)
		return;

	if (!csgo::local_player->is_alive())
	{
		path.clear();
		return;
	}

	constexpr auto bsp = 3;
	auto new_map = std::string(interfaces::clientstate->level_name);
	new_map.replace(new_map.size() - bsp, bsp, "nav");

	if (map != new_map || !valid)
	{
		map = new_map;
		valid = nav.load("csgo\\" + map);
		path.clear();
		if (!valid)
			return;
	}

	if (path.size() <= i || !target || !target->is_alive())
	{
		const auto new_target = get_nearest_player();
		if (new_target)
		{
			path = nav.find_path(csgo::local_player->origin(), new_target->abs_origin());
			target = new_target;
			i = 0;
		}
	}
	else
	{
		cmd->viewangles.x = 0;
		cmd->viewangles.y += std::clamp((math::calculate_angle(csgo::local_player->get_eye_pos(), path.at(i)) - cmd->viewangles).normalized_angles().y, -2.f, 2.f);
		interfaces::engine->set_view_angles(cmd->viewangles);
		const auto velocity = csgo::local_player->get_eye_pos() - path.at(i);
		auto direction = math::vector_angles(velocity);
		direction.y = cmd->viewangles.y - direction.y;
		const auto negated_direction = math::angle_vector(direction) * 1000;
		cmd->forwardmove = negated_direction.x;
		cmd->sidemove = negated_direction.y;

		if ((csgo::local_player->abs_origin() - path.at(i)).length_2d() < 10)
			i++;
	}
}