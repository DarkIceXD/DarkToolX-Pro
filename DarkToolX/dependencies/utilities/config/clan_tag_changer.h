#pragma once
#include "common.h"

namespace config {
	struct clan_tag {
		clan_tag()
		{
			name.reserve(15);
		}
		clan_tag(const char* name, const int delay, const std::vector<std::string>& tags) :
			name(name), delay(delay), tags(tags)
		{
			this->name.reserve(15);
		}
		std::string name{ "New Clan Tag" };
		int delay{ 400 };
		std::vector<std::string> tags{};
		JSON_SERIALIZE(clan_tag, name, delay, tags)
	};

	struct clan_tag_changer {
		clan_tag_changer()
		{
			conf_name.reserve(20);
		}
		std::string conf_name{ "New Config" };
		bool enabled{ false };
		size_t selected{ 0 };
		std::vector<clan_tag> tags{ {"DarkToolX", 400, {".........","........D",".......D.","......D..",".....D...","....D....","...D.....","..D......",".D.......","D........","D.......a","D......a.","D.....a..","D....a...","D...a....","D..a.....","D.a......","Da.......","Da......r","Da.....r.","Da....r..","Da...r...","Da..r....","Da.r.....","Dar......","Dar.....k","Dar....k.","Dar...k..","Dar..k...","Dar.k....","Dark.....","Dark....T","Dark...T.","Dark..T..","Dark.T...","DarkT....","DarkT...o","DarkT..o.","DarkT.o..","DarkTo...","DarkTo..o","DarkTo.o.","DarkToo..","DarkToo.l","DarkTool.","DarkToolX","DarkTool.","DarkToo.l","DarkToo..","DarkTo.o.","DarkTo..o","DarkTo...","DarkT.o..","DarkT..o.","DarkT...o","DarkT....","Dark.T...","Dark..T..","Dark...T.","Dark....T","Dark.....","Dar.k....","Dar..k...","Dar...k..","Dar....k.","Dar.....k","Dar......","Da.r.....","Da..r....","Da...r...","Da....r..","Da.....r.","Da......r","Da.......","D.a......","D..a.....","D...a....","D....a...","D.....a..","D......a.","D.......a","D........",".D.......","..D......","...D.....","....D....",".....D...","......D..",".......D.","........D"}} };
		clan_tag& get_selected()
		{
			if (tags.size() == 0)
				tags.push_back({ "DarkToolX", 400, {".........","........D",".......D.","......D..",".....D...","....D....","...D.....","..D......",".D.......","D........","D.......a","D......a.","D.....a..","D....a...","D...a....","D..a.....","D.a......","Da.......","Da......r","Da.....r.","Da....r..","Da...r...","Da..r....","Da.r.....","Dar......","Dar.....k","Dar....k.","Dar...k..","Dar..k...","Dar.k....","Dark.....","Dark....T","Dark...T.","Dark..T..","Dark.T...","DarkT....","DarkT...o","DarkT..o.","DarkT.o..","DarkTo...","DarkTo..o","DarkTo.o.","DarkToo..","DarkToo.l","DarkTool.","DarkToolX","DarkTool.","DarkToo.l","DarkToo..","DarkTo.o.","DarkTo..o","DarkTo...","DarkT.o..","DarkT..o.","DarkT...o","DarkT....","Dark.T...","Dark..T..","Dark...T.","Dark....T","Dark.....","Dar.k....","Dar..k...","Dar...k..","Dar....k.","Dar.....k","Dar......","Da.r.....","Da..r....","Da...r...","Da....r..","Da.....r.","Da......r","Da.......","D.a......","D..a.....","D...a....","D....a...","D.....a..","D......a.","D.......a","D........",".D.......","..D......","...D.....","....D....",".....D...","......D..",".......D.","........D"} });
			selected = std::clamp(selected, 0U, tags.size() - 1);
			return tags.at(selected);
		}
		JSON_SERIALIZE(clan_tag_changer, enabled, conf_name, selected, tags)
	};
};