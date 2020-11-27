#pragma once
#include "../../dependencies/interfaces/i_game_event_manager.hpp"

class event_listener : public i_game_event_listener2
{
public:
	event_listener();
	~event_listener();
	void fire_game_event(i_game_event* event) override;
};

