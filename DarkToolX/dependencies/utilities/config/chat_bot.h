#pragma once
#include "common.h"

namespace config {
	struct chat_bot_msg {
		chat_bot_msg()
		{
			msg.reserve(15);
		}
		std::string msg{ "get good, get DarkToolX" };
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(chat_bot_msg, msg)
	};

	struct chat_bot {
		chat_bot()
		{
			conf_name.reserve(20);
		}
		std::string conf_name{ "New Config" };
		bool enabled{ false };
		std::vector<chat_bot_msg> msgs;
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(chat_bot, enabled, conf_name, msgs)
	};
};