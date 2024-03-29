#include "features.hpp"

static void set_clan_tag(const char* tag)
{
	static auto fn_set_clan_tag = reinterpret_cast<int(__fastcall*)(const char*, const char*)>(utilities::pattern_scan("engine.dll", "53 56 57 8B DA 8B F9 FF 15"));
	if (fn_set_clan_tag)
		fn_set_clan_tag(tag, tag);
}

void features::clan_tag_changer()
{
	static auto changed_clan_tag = false;
	if (!csgo::cfg.clan_tag_changer().enabled)
	{
		if (changed_clan_tag)
		{
			set_clan_tag("");
			changed_clan_tag = false;
		}
		return;
	}

	const auto size = csgo::cfg.clan_tag_changer().get_selected().tags.size();
	if (size == 0)
		return;

	static size_t index = 0;
	const auto new_index = static_cast<size_t>(interfaces::globals->cur_time * 1000 / csgo::cfg.clan_tag_changer().get_selected().delay) % size;
	if (index != new_index || (size == 1 && !(interfaces::globals->tick_count % 100)))
	{
		set_clan_tag(csgo::cfg.clan_tag_changer().get_selected().tags.at(new_index).c_str());
		index = new_index;
		changed_clan_tag = true;
	}
}