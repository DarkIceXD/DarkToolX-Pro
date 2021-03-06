#include "features.hpp"

void features::ragdolls()
{
	for (auto i = 1; i <= interfaces::entity_list->get_highest_index(); ++i)
	{
		auto entity = static_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!entity || entity->dormant())
			continue;

		auto client_class = entity->client_class();
		if (!client_class || client_class->class_id != ccsragdoll)
			continue;

		auto ragdoll = (ragdoll_t*)entity;
		console::log("%f, %f, %f\n", ragdoll->force().x, ragdoll->force().y, ragdoll->force().z);
		/*ragdoll->force() *= 0;
		ragdoll->force().z = 1000;*/
	}
}