#include "features.hpp"

static std::array<int, 65> missed_shots;
struct shot_data {
	player_t* entity;
	vec3_t start, end;
	vec3_t origin, mins, maxs, eye;
	matrix_t matrix[256];
	bool wants_to_shoot, shot_valid, hit_target;
} data;

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
		return -(std::min)(max_delta, 35.f);
	case 4:
		return (std::min)(max_delta, 35.f);
	}
}

static void store_data(shot_data& data)
{
	data.origin = data.entity->abs_origin();
	data.eye = data.entity->abs_angles();
	data.mins = data.entity->collideable()->mins();
	data.maxs = data.entity->collideable()->maxs();
	data.entity->setup_bones(data.matrix, 256, BONE_USED_BY_ANYTHING, interfaces::globals->cur_time);
}

static void load_data(shot_data& data)
{
	auto& bone_cache = data.entity->get_cached_bones();
	std::memcpy(bone_cache.base(), data.matrix, bone_cache.count() * sizeof(matrix_t));
	data.entity->set_position(data.origin);
	data.entity->set_angles(data.eye);
	data.entity->collideable()->set_collision_bounds(data.mins, data.maxs);
}

void features::resolver::new_tick(c_usercmd* cmd)
{
	if (data.shot_valid && !data.hit_target)
	{
		data.shot_valid = false;
		shot_data backup;
		backup.entity = data.entity;
		store_data(backup);
		load_data(data);
		ray_t ray(data.start, data.end);
		trace_t tr;
		interfaces::trace_ray->clip_ray_to_entity(ray, MASK_SHOT, data.entity, &tr);
		if (!tr.did_hit())
		{
			const auto chat = interfaces::clientmode->get_hud_chat();
			if (chat)
				chat->printf(0, "[""\x03""DarkToolX\x01]\x01 Shot missed due to spread");
		}
		else
		{
			missed_shots[data.entity->index()]++;
			const auto chat = interfaces::clientmode->get_hud_chat();
			if (chat)
				chat->printf(0, "[""\x03""DarkToolX\x01]\x01 Shot missed due to Anti Aim. (%d)", missed_shots[data.entity->index()] % 5);
		}
		load_data(backup);
	}
	if (csgo::conf->misc().resolver && csgo::target.entity && csgo::want_to_shoot && !csgo::manual_shoot)
	{
		data.wants_to_shoot = true;
		data.hit_target = false;
		data.start = csgo::local_player->get_eye_pos();
		data.entity = csgo::target.entity;
		store_data(data);
	}
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

		const auto new_goal_feet_yaw = entity->eye_angles().y + get_delta(std::abs(entity->max_desync_angle()), missed_shots[i]);
		anim_state->goal_feet_yaw = std::isfinite(new_goal_feet_yaw) ? std::remainder(new_goal_feet_yaw, 360.0f) : 0.0f;
	}
}

void features::resolver::bullet_impact(i_game_event* event)
{
	if (!data.wants_to_shoot && !data.shot_valid)
		return;

	if (!csgo::local_player)
		return;

	if (interfaces::engine->get_player_for_user_id(event->get_int("userid")) != csgo::local_player->index())
		return;

	data.shot_valid = true;
	data.wants_to_shoot = false;
	data.end = { event->get_float("x"), event->get_float("y"), event->get_float("z") };
}

void features::resolver::player_hurt(i_game_event* event)
{
	if (!data.shot_valid)
		return;

	if (!csgo::local_player)
		return;

	const auto local_player_index = csgo::local_player->index();
	const auto victim = interfaces::engine->get_player_for_user_id(event->get_int("userid"));
	if (interfaces::engine->get_player_for_user_id(event->get_int("attacker")) != local_player_index ||
		victim == local_player_index)
		return;

	data.hit_target = true;
}