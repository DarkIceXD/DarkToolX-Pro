#pragma once
#include "aimbot.h"
#include "trigger.h"
#include "visuals.h"
#include "view.h"
#include "skinchanger.h"
#include "clan_tag_changer.h"
#include "misc.h"
#include <fstream>

struct conf
{
	static constexpr const char* conf_name = "DarkToolX.json";
	conf()
	{
		std::ifstream stream(conf_name);
		if (stream.good())
		{
			try {
				const auto json = nlohmann::json::parse(stream);
				try {
					json.at("s_aimbot").get_to(s_aimbot);
					json.at("_aimbot").get_to(_aimbot);
				}
				catch (nlohmann::json::exception) {}
				try {
					json.at("s_trigger").get_to(s_trigger);
					json.at("_trigger").get_to(_trigger);
				}
				catch (nlohmann::json::exception) {}
				try {
					s_visuals = json.value("s_visuals", 0);
					_visuals = json.value<std::vector<config::visuals>>("_visuals", { {} });
				}
				catch (nlohmann::json::exception) {}
				try {
					s_view = json.value("s_view", 0);
					_view = json.value<std::vector<config::view>>("_view", { {} });
				}
				catch (nlohmann::json::exception) {}
				try {
					s_skin_changer = json.value("s_skin_changer", 0);
					_skin_changer = json.value<std::vector<config::skin_changer>>("_skin_changer", { {} });
				}
				catch (nlohmann::json::exception) {}
				try {
					s_clan_tag_changer = json.value("s_clan_tag_changer", 0);
					_clan_tag_changer = json.value<std::vector<config::clan_tag_changer>>("_clan_tag_changer", { {} });
				}
				catch (nlohmann::json::exception) {}
				try {
					s_misc = json.value("s_misc", 0);
					_misc = json.value<std::vector<config::misc>>("_misc", { {} });
				}
				catch (nlohmann::json::exception) {}
			}
			catch (nlohmann::json::exception) {}
		}
	}
	void save() const
	{
		nlohmann::json json(*this);
		std::ofstream stream(conf_name);
		stream << json;
	}
	size_t s_aimbot{ 0 }, s_trigger{ 0 }, s_visuals{ 0 }, s_view{ 0 }, s_skin_changer{ 0 }, s_clan_tag_changer{ 0 }, s_misc{ 0 };
	std::vector<config::aimbot> _aimbot{ {} };
	std::vector<config::trigger> _trigger{ {} };
	std::vector<config::visuals> _visuals{ {} };
	std::vector<config::view> _view{ {} };
	std::vector<config::skin_changer> _skin_changer{ {} };
	std::vector<config::clan_tag_changer> _clan_tag_changer{ {} };
	std::vector<config::misc> _misc{ {} };
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
	config::misc& misc()
	{
		if (_misc.size() == 0)
			_misc.push_back({});
		s_misc = std::clamp(s_misc, 0U, _misc.size() - 1);
		return _misc.at(s_misc);
	}
	JSON_SERIALIZE(conf, s_aimbot, _aimbot, s_trigger, _trigger, s_visuals, _visuals, s_view, _view, s_skin_changer, _skin_changer, s_clan_tag_changer, _clan_tag_changer, s_misc, _misc)
};