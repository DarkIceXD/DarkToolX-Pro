#include "features.hpp"

static std::array<std::deque<record>, 65> records;

int time_to_ticks(const float time)
{
	return static_cast<int>(0.5f + time / interfaces::globals->interval_per_tick);
}

float get_lerp()
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

std::deque<record>& features::backtrack::get_records(const int player_index)
{
	return records[player_index];
}

void features::backtrack::restore_record(player_t* entity, const int record_index)
{
	const auto& record = records[entity->index()][record_index];
	auto& bone_cache = entity->get_cached_bones();
	std::memcpy(bone_cache.base(), record.matrix, bone_cache.count() * sizeof(matrix_t));
	entity->collideable()->mins() = record.mins;
	entity->collideable()->maxs() = record.maxs;
}

int features::backtrack::restore_tick_count(const int player_index, const int record_index)
{
	return time_to_ticks(records[player_index][record_index].simulation_time + get_lerp());
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
		rec.mins = entity->collideable()->mins();
		rec.maxs = entity->collideable()->maxs();
		rec.simulation_time = entity->simulation_time();
		entity->setup_bones(rec.matrix, 256, BONE_USED_BY_ANYTHING, interfaces::globals->cur_time);

		records[i].push_front(rec);

		while (records[i].size() > 3 && records[i].size() > static_cast<size_t>(time_to_ticks(static_cast<float>(csgo::conf->aimbot().backtrack_time_limit) / 1000.f)))
			records[i].pop_back();

		if (auto invalid = std::find_if(std::cbegin(records[i]), std::cend(records[i]), [](const record& rec) { return !valid(rec.simulation_time); }); invalid != std::cend(records[i]))
			records[i].erase(invalid, std::cend(records[i]));
	}
}

void features::backtrack::run(c_usercmd* cmd)
{
	if (!csgo::conf->aimbot().backtrack)
		return;

	if (!csgo::local_player)
		return;

	if (!csgo::manual_shoot)
		return;

	int best_target = 0;
	int best_record = 0;
	float best_fov = FLT_MAX;
	const auto local_head = csgo::local_player->get_eye_pos();
	const auto view_angles = cmd->viewangles + csgo::local_player->recoil();
	for (int i = 1; i <= interfaces::globals->max_clients; i++)
	{
		auto entity = static_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity || entity == csgo::local_player || entity->dormant() || !entity->is_alive())
			continue;

		if (!csgo::local_player->is_enemy(entity))
			continue;

		for (size_t j = 3; j < records[i].size(); j++)
		{
			const auto& record = records[i][j];
			if (!valid(record.simulation_time))
				continue;

			const auto fov = math::fov(view_angles, math::calculate_angle(local_head, record.origin));
			if (fov < best_fov)
			{
				best_fov = fov;
				best_target = i;
				best_record = j;
			}
		}
	}

	if (best_target && best_record)
		cmd->tick_count = restore_tick_count(best_target, best_record);
}