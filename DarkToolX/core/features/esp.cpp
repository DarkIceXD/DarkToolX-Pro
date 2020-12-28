#include "features.hpp"

bool bounding_box(entity_t* entity, ImVec2& p_min, ImVec2& p_max)
{
	auto collideable = entity->collideable();
	if (!collideable)
		return false;

	const auto& min = collideable->mins();
	const auto& max = collideable->maxs();
	const vec3_t points[] = {
		min,
		{min.x, max.y, min.z},
		{max.x, max.y, min.z},
		{max.x, min.y, min.z},
		max,
		{min.x, max.y, max.z},
		{min.x, min.y, max.z},
		{max.x, min.y, max.z}
	};

	const auto& trans = entity->rgfl_coordinate_frame();
	vec2_t screen_points[8];
	for (int i = 0; i < 8; i++) {
		if (!math::world_to_screen(math::transform_vector(points[i], trans), screen_points[i]))
			return false;
	}

	p_min = { screen_points[0].x, screen_points[0].y };
	p_max = { screen_points[0].x, screen_points[0].y };
	for (int i = 1; i < 8; i++) {
		if (p_min.x > screen_points[i].x)
			p_min.x = screen_points[i].x;
		if (p_min.y < screen_points[i].y)
			p_min.y = screen_points[i].y;
		if (p_max.x < screen_points[i].x)
			p_max.x = screen_points[i].x;
		if (p_max.y > screen_points[i].y)
			p_max.y = screen_points[i].y;
	}
	return true;
}

void features::esp(ImDrawList* draw_list)
{
	if (!csgo::conf->visuals().box_esp)
		return;

	const auto team_color = csgo::conf->visuals().box_team.to_u32();
	const auto enemy_color = csgo::conf->visuals().box_enemy.to_u32();
	for (auto i = 1; i <= interfaces::globals->max_clients; i++)
	{
		auto entity = static_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity || !entity->is_player() || entity == csgo::local_player)
			continue;

		if (!csgo::conf->visuals().dormant_esp && entity->dormant())
			continue;

		const auto hp = entity->health();
		if (hp < 1)
			continue;

		ImVec2 min, max;
		if (!bounding_box(entity, min, max))
			continue;

		draw_list->AddRect(min, max, csgo::local_player->is_enemy(entity) ? enemy_color : team_color);
		ImVec2 hp_min, hp_max;
		hp_min.x = max.x + 3;
		hp_min.y = min.y;
		hp_max.x = hp_min.x + 2;
		const auto scale = static_cast<float>(hp) / entity->max_health();
		const auto health_based_y = (max.y - min.y) * scale;
		hp_max.y = hp_min.y + health_based_y;
		draw_list->AddRectFilled(hp_min, hp_max, config::rgb::scale({ 0, 1, 0, 1 }, { 1, 0, 0, 1 }, scale).to_u32());
		hp_min.x -= 1;
		hp_max.x += 1;
		draw_list->AddRect(hp_min, hp_max, IM_COL32_BLACK);
		player_info_t info;
		interfaces::engine->get_player_info(entity->index(), &info);
		const auto name_size = ImGui::CalcTextSize(info.name);
		draw_list->AddText({ min.x + (max.x - min.x - name_size.x) / 2, max.y - name_size.y - 2 }, IM_COL32_WHITE, info.name);
	}
}