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
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(trigger, conf_name, bind)
	};
};