#pragma once
#include "aimbot.h"
#include "trigger.h"
#include "visuals.h"
#include "view.h"
#include "skinchanger.h"
#include "clan_tag_changer.h"
#include "chat_bot.h"
#include "misc.h"
#include "logs.h"
#include <fstream>
#include <sstream>

struct conf
{
	static constexpr const char* conf_name = "DarkToolX.json";
	conf()
	{
		std::ifstream stream(conf_name);
		if (stream.good())
		{
			const auto json = nlohmann::json::parse(stream, nullptr, false);
			if (!json.is_discarded())
			{
				s_aimbot = json.value("s_aimbot", 0);
				_aimbot = json.value<std::vector<config::aimbot>>("_aimbot", { {} });

				s_trigger = json.value("s_trigger", 0);
				_trigger = json.value<std::vector<config::trigger>>("_trigger", { {} });

				s_visuals = json.value("s_visuals", 0);
				_visuals = json.value<std::vector<config::visuals>>("_visuals", { {} });

				s_view = json.value("s_view", 0);
				_view = json.value<std::vector<config::view>>("_view", { {} });

				s_skin_changer = json.value("s_skin_changer", 0);
				_skin_changer = json.value<std::vector<config::skin_changer>>("_skin_changer", { {} });

				s_clan_tag_changer = json.value("s_clan_tag_changer", 0);
				_clan_tag_changer = json.value<std::vector<config::clan_tag_changer>>("_clan_tag_changer", { {} });

				s_chat_bot = json.value("s_chat_bot", 0);
				_chat_bot = json.value<std::vector<config::chat_bot>>("_chat_bot", { {} });

				s_misc = json.value("s_misc", 0);
				_misc = json.value<std::vector<config::misc>>("_misc", { {} });

				s_logs = json.value("s_logs", 0);
				_logs = json.value<std::vector<config::logs>>("_logs", { {} });
			}
		}
	}
	void save() const
	{
		nlohmann::json json(*this);
		std::ofstream stream(conf_name);
		stream << json;
	}
	std::string export_config(const int i)
	{
		nlohmann::json json;
		switch (i)
		{
		case 1:
			json = trigger();
			break;
		case 2:
			json = visuals();
			break;
		case 3:
			json = view();
			break;
		case 4:
			json = skin_changer();
			break;
		case 5:
			json = clan_tag_changer();
			break;
		case 6:
			json = chat_bot();
			break;
		case 7:
			json = misc();
			break;
		case 8:
			json = logs();
			break;
		default:
			json = aimbot();
			break;
		}
		json["i"] = i;
		std::stringstream ss;
		ss << json;
		return ss.str();
	}
	void import_config(const std::string& shared_config)
	{
		const auto json = nlohmann::json::parse(shared_config, nullptr, false);
		if (!json.is_discarded() && json.contains("i"))
		{
			const auto i = json.at("i").get<int>();
			switch (i)
			{
			case 1:
				_trigger.push_back(json);
				break;
			case 2:
				_visuals.push_back(json);
				break;
			case 3:
				_view.push_back(json);
				break;
			case 4:
				_skin_changer.push_back(json);
				break;
			case 5:
				_clan_tag_changer.push_back(json);
				break;
			case 6:
				_chat_bot.push_back(json);
				break;
			case 7:
				_misc.push_back(json);
				break;
			case 8:
				_logs.push_back(json);
				break;
			default:
				_aimbot.push_back(json);
				break;
			}
		}
	}
	size_t s_aimbot{ 0 }, s_trigger{ 0 }, s_visuals{ 0 }, s_view{ 0 }, s_skin_changer{ 0 }, s_clan_tag_changer{ 0 }, s_chat_bot{ 0 }, s_misc{ 0 }, s_logs{ 0 };
	std::vector<config::aimbot> _aimbot{ {} };
	std::vector<config::trigger> _trigger{ {} };
	std::vector<config::visuals> _visuals{ {} };
	std::vector<config::view> _view{ {} };
	std::vector<config::skin_changer> _skin_changer{ {} };
	std::vector<config::clan_tag_changer> _clan_tag_changer{ {} };
	std::vector<config::chat_bot> _chat_bot{ {} };
	std::vector<config::misc> _misc{ {} };
	std::vector<config::logs> _logs{ {} };
	config::aimbot& aimbot()
	{
		if (_aimbot.size() == 0)
			_aimbot.push_back({});
		s_aimbot = std::clamp(s_aimbot, 0U, _aimbot.size() - 1);
		return _aimbot.at(s_aimbot);
	}
	config::trigger& trigger()
	{
		if (_trigger.size() == 0)
			_trigger.push_back({});
		s_trigger = std::clamp(s_trigger, 0U, _trigger.size() - 1);
		return _trigger.at(s_trigger);
	}
	config::visuals& visuals()
	{
		if (_visuals.size() == 0)
			_visuals.push_back({});
		s_visuals = std::clamp(s_visuals, 0U, _visuals.size() - 1);
		return _visuals.at(s_visuals);
	}
	config::view& view()
	{
		if (_view.size() == 0)
			_view.push_back({});
		s_view = std::clamp(s_view, 0U, _view.size() - 1);
		return _view.at(s_view);
	}
	config::skin_changer& skin_changer()
	{
		if (_skin_changer.size() == 0)
			_skin_changer.push_back({});
		s_skin_changer = std::clamp(s_skin_changer, 0U, _skin_changer.size() - 1);
		return _skin_changer.at(s_skin_changer);
	}
	config::clan_tag_changer& clan_tag_changer()
	{
		if (_clan_tag_changer.size() == 0)
			_clan_tag_changer.push_back({});
		s_clan_tag_changer = std::clamp(s_clan_tag_changer, 0U, _clan_tag_changer.size() - 1);
		return _clan_tag_changer.at(s_clan_tag_changer);
	}
	config::chat_bot& chat_bot()
	{
		if (_chat_bot.size() == 0)
			_chat_bot.push_back({});
		s_chat_bot = std::clamp(s_chat_bot, 0U, _chat_bot.size() - 1);
		return _chat_bot.at(s_chat_bot);
	}
	config::misc& misc()
	{
		if (_misc.size() == 0)
			_misc.push_back({});
		s_misc = std::clamp(s_misc, 0U, _misc.size() - 1);
		return _misc.at(s_misc);
	}
	config::logs& logs()
	{
		if (_logs.size() == 0)
			_logs.push_back({});
		s_logs = std::clamp(s_logs, 0U, _logs.size() - 1);
		return _logs.at(s_logs);
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(conf, s_aimbot, _aimbot, s_trigger, _trigger, s_visuals, _visuals, s_view, _view, s_skin_changer, _skin_changer, s_clan_tag_changer, _clan_tag_changer, s_chat_bot, _chat_bot, s_misc, _misc, s_logs, _logs)
};