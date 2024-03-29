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
		int pitch{ 89 };
		int yaw{ 160 };
		int desync{ 0 };
		int max_desync_delta{ 60 };
		bool fake_lag{ false };
		int fake_lag_ticks{ 1 };
		bool bhop{ true };
		bool auto_strafe{ false };
		bool no_duck_delay{ true };
		bool slow_walk{ false };
		bool auto_stop{ false };
		keybind fake_duck{};
		bool walk_bot{ false };
		bool hitsound{ false };
		bool quick_switch{ false };
		bool auto_accept{ true };
		bool reveal_ranks{ true };
		bool resolver{ false };
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(misc, conf_name, anti_aim, smart_anti_aim, pitch, yaw, desync, max_desync_delta, fake_lag, fake_lag_ticks, bhop, auto_strafe, no_duck_delay, slow_walk, auto_stop, fake_duck, walk_bot, hitsound, quick_switch, auto_accept, reveal_ranks, resolver)
	};
};