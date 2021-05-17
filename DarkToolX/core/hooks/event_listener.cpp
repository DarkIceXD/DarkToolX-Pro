#include "event_listener.h"
#include "../../dependencies/utilities/csgo.hpp"
#include "../features/features.hpp"

event_listener::event_listener()
{
	debug_id = EVENT_DEBUG_ID_INIT;
	interfaces::event_manager->add_listener(this, "player_hurt", false);
	interfaces::event_manager->add_listener(this, "player_death", false);
	interfaces::event_manager->add_listener(this, "player_footstep", false);
	interfaces::event_manager->add_listener(this, "bullet_impact", false);
	interfaces::event_manager->add_listener(this, "vote_cast", false);
	interfaces::event_manager->add_listener(this, "item_pickup", false);
}

event_listener::~event_listener()
{
	interfaces::event_manager->remove_listener(this);
}

void event_listener::fire_game_event(i_game_event* event)
{
	switch (fnv::hash(event->get_name())) {
	case fnv::hash("player_hurt"):
		features::resolver::player_hurt(event);
		features::events::hit_sound(event);
		features::logs::player_hurt(event);
		break;
	case fnv::hash("player_death"):
		features::events::update_stattrak(event);
		features::events::chat_bot(event);
		features::logs::player_death(event);
		break;
	case fnv::hash("player_footstep"):
		features::step_esp::player_footstep(event);
		break;
	case fnv::hash("bullet_impact"):
		features::bullet_tracers::bullet_impact(event);
		features::resolver::bullet_impact(event);
		break;
	case fnv::hash("vote_cast"):
		features::logs::vote_cast(event);
		break;
	case fnv::hash("item_pickup"):
		features::logs::item_pickup(event);
		break;
	}
}
