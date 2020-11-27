#include "event_listener.h"
#include "../../dependencies/utilities/csgo.hpp"
#include "../features/features.hpp"

event_listener::event_listener()
{
	debug_id = 42;
	interfaces::event_manager->add_listener(this, "player_hurt", false);
	interfaces::event_manager->add_listener(this, "player_death", false);
	interfaces::event_manager->add_listener(this, "player_footstep", false);
}

event_listener::~event_listener()
{
	interfaces::event_manager->remove_listener(this);
}

void event_listener::fire_game_event(i_game_event* event)
{
	switch (fnv::hash(event->get_name())) {
	case fnv::hash("player_hurt"):
		features::events::hit_sound(event);
		break;
	case fnv::hash("player_death"):
		features::events::update_stattrak(event);
		features::events::chat_bot(event);
		break;
	case fnv::hash("player_footstep"):
		features::events::step_esp(event);
		break;
	}
}
