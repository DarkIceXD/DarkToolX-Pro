#pragma once
#include "common.h"

namespace config {
	struct trigger {
		trigger()
		{
			conf_name.reserve(20);
		}
		std::string conf_name{ "New Config" };
		bool enabled{ false };
		int key_bind_type{ 0 };
		int key_bind{ 0x12 };
		JSON_SERIALIZE(trigger, enabled, key_bind_type, key_bind)
	};
};