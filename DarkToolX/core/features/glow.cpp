#include "features.hpp"

void features::glow()
{
	if (!csgo::local_player)
		return;

	const auto glow_objects = interfaces::glow_manager->objects;
	for (auto i = 0; i < interfaces::glow_manager->size; i++)
	{
		auto entity = static_cast<entity_t*>(glow_objects[i].entity);
		if (!entity || entity->dormant())
			continue;
		const auto class_id = entity->client_class()->class_id;
		const auto is_enemy = csgo::local_player->is_enemy(entity);
		const auto rgb = csgo::conf->visuals().get_rgb(class_id, is_enemy);
		if (rgb && rgb->enabled)
		{
			if (class_id == ccsplayer && is_enemy && csgo::conf->visuals().health_based)
			{
				const auto hp = static_cast<float>(static_cast<player_t*>(entity)->health()) / entity->max_health();
				const auto rgb_scaled = config::rgb::scale(*rgb, csgo::conf->visuals().glow.back(), hp);
				glow_objects[i].set(rgb_scaled.r, rgb_scaled.g, rgb_scaled.b, rgb_scaled.a);
			}
			else
			{
				glow_objects[i].set(rgb->r, rgb->g, rgb->b, rgb->a);
			}
		}
	}
}