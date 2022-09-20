#pragma once
#include "common.h"

namespace config {
	struct log {
		bool enabled{ false };
		bool local_player{ false };
		bool team{ false };
		bool enemy{ false };
		constexpr bool is_enabled(const bool team, const bool is_local_player) const
		{
			return team ? (is_local_player ? this->local_player : this->team) : this->enemy;
		}
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(log, enabled, local_player, team, enemy)
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
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(logs, conf_name, deaths, damage, pickup, votes)
	};
};