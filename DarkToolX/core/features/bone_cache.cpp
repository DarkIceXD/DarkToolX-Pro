#include "features.hpp"

struct cache_item {
	matrix_t bone_matrices[MAXSTUDIOBONES];
};

static std::unordered_map<int, cache_item> cache;

void features::bone_cache::clear()
{
	cache.clear();
}

static cache_item& get_matrix(player_t* entity)
{
	const auto i = entity->get_index();
	if (!cache.count(i))
	{
		cache_item item = {};
		entity->setup_bones(item.bone_matrices, 128, 256, 0.0f);
		cache[i] = item;
	}
	return cache.at(i);
}

vec3_t features::bone_cache::get_bone(player_t* entity, int bone)
{
	const auto& item = get_matrix(entity);
	return { item.bone_matrices[bone][0][3], item.bone_matrices[bone][1][3], item.bone_matrices[bone][2][3] };
}

vec3_t features::bone_cache::get_hitbox(player_t* entity, int hitbox_id)
{
	const auto studio_model = interfaces::model_info->get_studio_model(entity->model());
	if (studio_model) {
		const auto hitbox = studio_model->hitbox_set(0)->hitbox(hitbox_id);
		if (hitbox) {
			const auto& item = get_matrix(entity);
			const auto min = math::transform_vector(hitbox->mins, item.bone_matrices[hitbox->bone]);
			const auto max = math::transform_vector(hitbox->maxs, item.bone_matrices[hitbox->bone]);
			return vec3_t((min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f, (min.z + max.z) * 0.5f);
		}
	}
	return {};
}