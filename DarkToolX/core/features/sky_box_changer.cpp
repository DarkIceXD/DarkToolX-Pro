#include "features.hpp"

void load_skybox(const char* sky)
{
	static auto fn_load_skybox = reinterpret_cast<void(__fastcall*)(const char*)>(utilities::pattern_scan("engine.dll", "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45"));
	if (fn_load_skybox)
		fn_load_skybox(sky);
}

void features::sky_box_changer()
{
	static auto is_defaut = true;
	if (csgo::cfg.view().sky_box > 0)
	{
		load_skybox(sky_list.at(csgo::cfg.view().sky_box));
		is_defaut = false;
	}
	else if(!is_defaut)
	{
		static const auto sv_skyname = interfaces::console->get_convar("sv_skyname");
		load_skybox(sv_skyname->string);
		is_defaut = true;
	}
}