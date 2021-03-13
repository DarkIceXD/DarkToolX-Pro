#include "features.hpp"
#include <deque>

constexpr auto time_limit = 200;
constexpr auto mode = 1;

struct record {
	vec3_t origin;
	vec3_t view_angles;
	float simulation_time;
	// matrix_t matrix[256];
};

static std::array<std::deque<record>, 65> records;

int features::backtrack::time_to_ticks(const float time)
{
	return static_cast<int>(0.5f + time / interfaces::globals->interval_per_tick);
}

float features::backtrack::get_lerp()
{
	static auto interp_ratio = interfaces::console->get_convar("cl_interp_ratio");
	static auto min_interp_ratio = interfaces::console->get_convar("sv_client_min_interp_ratio");
	static auto max_interp_ratio = interfaces::console->get_convar("sv_client_max_interp_ratio");
	const auto ratio = std::clamp(interp_ratio->get_float(), min_interp_ratio->get_float(), max_interp_ratio->get_float());

	static auto interp = interfaces::console->get_convar("cl_interp");
	static auto max_update_rate = interfaces::console->get_convar("sv_maxupdaterate");
	static auto update_rate = interfaces::console->get_convar("cl_updaterate");
	return (std::max)(interp->get_float(), (ratio / ((max_update_rate) ? max_update_rate->get_float() : update_rate->get_float())));
}

bool features::backtrack::valid(const float simtime)
{
	const auto network = interfaces::engine->get_net_channel_info();
	if (!network)
		return false;

	static auto max_unlag = interfaces::console->get_convar("sv_maxunlag");
	const auto delta = std::clamp(network->get_latency(0) + network->get_latency(1) + get_lerp(), 0.f, max_unlag->get_float()) - (interfaces::globals->cur_time - simtime);
	return std::abs(delta) <= 0.2f;
}

void features::backtrack::update()
{
	if (!csgo::conf->aimbot().backtrack)
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive())
	{
		for (auto& record : records)
			record.clear();
		return;
	}

	for (auto i = 1; i <= interfaces::globals->max_clients; i++)
	{
		auto entity = static_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity || entity == csgo::local_player || entity->dormant() || !entity->is_alive() || !csgo::local_player->is_enemy(entity))
		{
			records[i].clear();
			continue;
		}

		if (!records[i].empty() && (records[i].front().simulation_time == entity->simulation_time()))
			continue;

		record rec{ };
		rec.origin = entity->abs_origin();
		rec.view_angles = entity->abs_angles();
		rec.simulation_time = entity->simulation_time();

		//entity->setup_bones(rec.matrix, 256, BONE_USED_BY_ANYTHING, interfaces::globals->cur_time);

		records[i].push_front(rec);

		while (records[i].size() > 3 && records[i].size() > static_cast<size_t>(time_to_ticks(static_cast<float>(csgo::conf->aimbot().backtrack_time_limit) / 1000.f)))
			records[i].pop_back();

		if (auto invalid = std::find_if(std::cbegin(records[i]), std::cend(records[i]), [](const record& rec) { return !valid(rec.simulation_time); }); invalid != std::cend(records[i]))
			records[i].erase(invalid, std::cend(records[i]));
	}
}

vec3_t calc_relative_angle(const vec3_t& source, const vec3_t& destination, const vec3_t& view_angles)
{
	return (math::calculate_angle(source, destination) - view_angles).normalized_angles();
}

static void rage_backtrack(c_usercmd* cmd)
{
	if (csgo::want_to_shoot)
		return;

	const auto time = interfaces::globals->interval_per_tick * cmd->tick_count;
	if (csgo::local_player->next_attack() > time)
		return;

	const auto weapon = csgo::local_player->active_weapon();
	if (!weapon)
		return;

	if (weapon->clip1_count() < 1)
		return;

	const auto weapon_data = weapon->get_weapon_data();
	if (!weapon_data)
		return;

	if (!weapon_data->weapon_full_auto && weapon->next_primary_attack() > time)
		return;

	int best_target_index{};
	int best_record{};
	best_target target = {};
	const auto local_head = csgo::local_player->get_eye_pos();
	const auto aimpunch = csgo::local_player->recoil();
	for (int i = 1; i <= interfaces::globals->max_clients; i++)
	{
		auto entity = static_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity || entity == csgo::local_player || entity->dormant())
			continue;

		const auto hp = entity->health();
		if (hp < 1)
			continue;

		if (entity->has_gun_game_immunity())
			continue;

		if (!csgo::local_player->is_enemy(entity))
			continue;

		if (records[i].size() <= 3)
			continue;

		const auto abs_angles = entity->abs_angles();
		const auto abs_origin = entity->abs_origin();
		for (size_t j = 0; j < records[i].size(); j++)
		{
			const auto& record = records[i][j];
			if (!features::backtrack::valid(record.simulation_time))
				continue;

			entity->set_angles(record.view_angles);
			entity->set_position(record.origin);
			constexpr int hitboxes[] = { hitbox_pelvis, hitbox_stomach, hitbox_head };
			for (const auto k : hitboxes)
			{
				auto current_hitbox = entity->get_hitbox_position(k);
				const auto new_viewangle = (math::calculate_angle(local_head, current_hitbox) - aimpunch).normalized_angles();
				if (math::fov(cmd->viewangles, new_viewangle) > csgo::conf->aimbot().fov)
					continue;
				const auto data = features::auto_wall(current_hitbox - local_head, weapon_data, false);
				if (data.damage > target.damage)
				{
					target.entity = data.entity;
					target.damage = static_cast<int>(data.damage);
					target.angle = new_viewangle;
					target.lethal = target.damage >= hp;
					target.hitbox = k;
					best_target_index = i;
					best_record = j;
					if (target.lethal)
						break;
				}
			}
			if (target.lethal)
				break;
		}
		if (target.lethal)
			break;
		entity->set_angles(abs_angles);
		entity->set_position(abs_origin);
	}

	if (!best_target_index || !best_record)
		return;
	
	const auto weapon_setting = csgo::conf->aimbot().get_weapon_settings(weapon->item_definition_index());
	if (!(target.lethal || target.damage >= (csgo::conf->aimbot().min_dmg_override_active ? weapon_setting.min_dmg_override : weapon_setting.min_dmg)))
		return;
	//das ist das problem
	/*if (!features::hitchance(target.angle, target.entity, weapon_setting.hitchance, weapon, weapon_data, features::hitbox_to_hitgroup(target.hitbox)))
		return;
*/
	csgo::target = target;
	const auto& record = records[best_target_index][best_record];
	cmd->tick_count = features::backtrack::time_to_ticks(record.simulation_time + features::backtrack::get_lerp());
	cmd->viewangles = target.angle;
	cmd->buttons |= in_attack;
	csgo::want_to_shoot = true;
}

void features::backtrack::run(c_usercmd* cmd)
{
	if (!csgo::conf->aimbot().backtrack)
		return;

	if (!csgo::local_player)
		return;

	if (!csgo::manual_shoot)
		return;

	auto best_fov{ 255.f };
	entity_t* best_target{ };
	int best_record{ };
	int best_target_index;
	vec3_t best_target_origin;

	const auto local_head = csgo::local_player->get_eye_pos();
	const auto view_angles = cmd->viewangles + csgo::local_player->recoil();
	for (int i = 1; i <= interfaces::globals->max_clients; i++)
	{
		auto entity = static_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity || entity == csgo::local_player || entity->dormant() || !entity->is_alive())
			continue;

		if (!csgo::local_player->is_enemy(entity))
			continue;

		const auto& origin = entity->abs_origin();

		const auto angle = calc_relative_angle(local_head, origin, view_angles);
		const auto fov = std::hypotf(angle.x, angle.y);
		if (fov < best_fov)
		{
			best_fov = fov;
			best_target = entity;
			best_target_index = i;
			best_target_origin = origin;
		}
	}

	if (best_target)
	{
		if (records[best_target_index].size() <= 3)
			return;

		best_fov = 255.f;
		for (size_t i = 0; i < records[best_target_index].size(); i++)
		{
			const auto& record = records[best_target_index][i];
			if (!features::backtrack::valid(record.simulation_time))
				continue;

			const auto angle = calc_relative_angle(local_head, record.origin, view_angles);
			const auto fov = std::hypotf(angle.x, angle.y);
			if (fov < best_fov)
			{
				best_fov = fov;
				best_record = i;
			}
		}
	}

	if (best_record)
	{
		const auto& record = records[best_target_index][best_record];
		cmd->tick_count = features::backtrack::time_to_ticks(record.simulation_time + features::backtrack::get_lerp());
	}
}