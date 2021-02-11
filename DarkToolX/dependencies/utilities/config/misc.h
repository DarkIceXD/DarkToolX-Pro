#pragma once
#include "common.h"

namespace config {
	struct misc
	{
		misc()
		{
			conf_name.reserve(20);
		}
		std::string conf_name{ "New Config" };
		int anti_aim{ 0 };
		bool smart_anti_aim{ false };
		bool fake_lag{ false };
		int fake_lag_ticks{ 1 };
		bool bhop{ true };
		bool no_duck_delay{ true };
		bool slow_walk{ false };
		bool hitsound{ false };
		bool auto_switch{ false };
		bool auto_accept{ true };
		bool reveal_ranks{ true };
		JSON_SERIALIZE(misc, conf_name, anti_aim, smart_anti_aim, fake_lag, fake_lag_ticks, bhop, no_duck_delay, slow_walk, hitsound, auto_switch, auto_accept, reveal_ranks)
	};
};