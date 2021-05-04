#pragma once
#include "common.h"

namespace config {
	struct log {
		bool enabled{ false };
		bool local_player{ false };
		bool team{ false };
		bool enemy{ false };
		JSON_SERIALIZE(log, enabled, local_player, team, enemy)
	};

	struct logs
	{
		logs()
		{
			conf_name.reserve(20);
		}
		std::string conf_name{ "New Config" };
		log deaths{ };
		log damage{ };
		log pickup{ };
		bool votes{ false };
		JSON_SERIALIZE(logs, conf_name, deaths, damage, pickup, votes)
	};
};