#include "features.hpp"

struct entity_data {
	struct bone {
		vec3_t from, to;
	};
	ImVec2 min, max;
	float health;
	float armor;
	char name[128];
	const char* weapon_name;
	std::vector<bone> bones;
	bool enemy;
	bool has_heavy_armor;
	bool draw;
};

static std::array<entity_data, 65> entities;

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

void features::esp::update()
{
	if (!csgo::local_player)
		return;

	for (auto i = 1; i <= interfaces::globals->max_clients; i++)
	{
		auto& data = entities[i];
		data.draw = false;

		if (!csgo::cfg.visuals().esp)
			continue;

		auto entity = static_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity || !entity->is_player() || entity == csgo::local_player)
			continue;

		if (!csgo::cfg.visuals().dormant_esp && entity->dormant())
			continue;

		const auto hp = entity->health();
		if (hp < 1)
			continue;

		if (!bounding_box(entity, data.min, data.max))
			continue;

		data.health = static_cast<float>(hp) / entity->max_health();
		data.armor = entity->armor() / 100.f;
		const auto weapon = entity->active_weapon();
		data.weapon_name = weapon ? weapon->get_weapon_name() : nullptr;
		data.enemy = csgo::local_player->is_enemy(entity);
		data.has_heavy_armor = entity->has_heavy_armor();
		player_info_t info;
		interfaces::engine->get_player_info(entity->index(), &info);
		strcpy_s(data.name, info.name);

		data.bones.clear();
		matrix_t bone_matrices[MAXSTUDIOBONES];
		if (entity->setup_bones(bone_matrices, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0))
		{
			const auto studio_model = interfaces::model_info->get_studio_model(entity->model());
			for (int i = 0; i < studio_model->bones_count; i++) {
				const auto bone = studio_model->bone(i);

				if (!bone || bone->parent == -1 || !(bone->flags & BONE_USED_BY_HITBOX))
					continue;

				data.bones.push_back({ { bone_matrices[i][0][3], bone_matrices[i][1][3], bone_matrices[i][2][3] }, { bone_matrices[bone->parent][0][3], bone_matrices[bone->parent][1][3], bone_matrices[bone->parent][2][3] } });
			}
		}

		data.draw = true;
	}
}

void features::esp::draw(ImDrawList* draw_list)
{
	if (!csgo::local_player)
		return;

	const auto team_color = csgo::cfg.visuals().box_team.to_u32();
	const auto enemy_color = csgo::cfg.visuals().box_enemy.to_u32();
	const auto skeleton_team_color = csgo::cfg.visuals().skeleton_team.to_u32();
	const auto skeleton_enemy_color = csgo::cfg.visuals().skeleton_enemy.to_u32();
	for (const auto& entity : entities)
	{
		if (!entity.draw)
			continue;

		draw_list->AddRect(entity.min, entity.max, entity.enemy ? enemy_color : team_color);
		{
			ImVec2 hp_min, hp_max;
			hp_min.x = entity.max.x + 3;
			hp_min.y = entity.min.y;
			hp_max.x = hp_min.x + 2;
			const auto health_based_y = (entity.max.y - entity.min.y) * entity.health;
			hp_max.y = hp_min.y + health_based_y;
			draw_list->AddRectFilled(hp_min, hp_max, config::rgb::scale({ 0, 1, 0, 1 }, { 1, 0, 0, 1 }, entity.health).to_u32());
			hp_min.x -= 1;
			hp_max.x += 1;
			draw_list->AddRect(hp_min, hp_max, IM_COL32_BLACK);
		}
		if (entity.armor > 0)
		{
			ImVec2 armor_min, armor_max;
			armor_min.x = entity.min.x;
			armor_min.y = entity.min.y + 3;
			const auto armor_based_x = (entity.max.x - entity.min.x) * entity.armor;
			armor_max.x = armor_min.x + armor_based_x;
			armor_max.y = armor_min.y + 2;
			draw_list->AddRectFilled(armor_min, armor_max, entity.has_heavy_armor ? IM_COL32(255, 128, 0, 255) : IM_COL32(0, 85, 255, 255));
			armor_min.y -= 1;
			armor_max.y += 1;
			draw_list->AddRect(armor_min, armor_max, IM_COL32_BLACK);
		}
		const auto name_size = ImGui::CalcTextSize(entity.name);
		draw_list->AddText({ entity.min.x + (entity.max.x - entity.min.x - name_size.x) / 2, entity.max.y - name_size.y - 2 }, IM_COL32_WHITE, entity.name);
		if (entity.weapon_name)
		{
			const auto weapon_size = ImGui::CalcTextSize(entity.weapon_name);
			draw_list->AddText({ entity.min.x + (entity.max.x - entity.min.x - weapon_size.x) / 2, entity.min.y + weapon_size.y }, IM_COL32_WHITE, entity.weapon_name);
		}
		for (const auto& bone : entity.bones)
		{
			vec2_t bone_point, parent_point;
			if (!math::world_to_screen(bone.from, bone_point) || !math::world_to_screen(bone.to, parent_point))
				continue;

			draw_list->AddLine({ bone_point.x, bone_point.y }, { parent_point.x, parent_point.y }, entity.enemy ? skeleton_enemy_color : skeleton_team_color);
		}
	}

	if (csgo::cfg.visuals().aimbot_spot)
	{
		if (csgo::target.entity)
		{
			vec2_t end_screen;
			if (math::world_to_screen(csgo::target.aim_spot, end_screen))
			{
				draw_list->AddLine({ end_screen.x - 5, end_screen.y - 5 }, { end_screen.x + 5, end_screen.y + 5 }, IM_COL32_WHITE);
				draw_list->AddLine({ end_screen.x - 5, end_screen.y + 5 }, { end_screen.x + 5, end_screen.y - 5 }, IM_COL32_WHITE);
			}
		}
	}
}