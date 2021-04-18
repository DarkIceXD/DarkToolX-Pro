#pragma once
#include "common.h"

namespace config {
	struct trigger {
		trigger()
		{
			conf_name.reserve(20);
		}
		std::string conf_name{ "New Config" };
		keybind bind{};
		JSON_SERIALIZE(trigger, conf_name, bind)
	};
};