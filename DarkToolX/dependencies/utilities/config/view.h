#pragma once
#include "common.h"

namespace config {
	struct view
	{
		view()
		{
			conf_name.reserve(20);
		}
		std::string conf_name{ "New Config" };
		keybind thirdperson{};
		float range{ 100.f };
		bool spectator_thirdperson{ false };
		bool crosshair{ true };
		bool show_recoil{ true };
		bool remove_scope{ true };
		int sky_box{ 0 };
		bool dark_skybox{ false };
		bool night_mode{ false };
		bool asus_walls{ false };
		float asus_walls_alpha{ 0.5f };
		bool no_fog{ false };
		bool no_flash{ true };
		float view_fov{ 0.f };
		int modify_smoke{ 0 };
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(view, conf_name, thirdperson, range, spectator_thirdperson, crosshair, show_recoil, remove_scope, sky_box, dark_skybox, night_mode, asus_walls, asus_walls_alpha, no_fog, no_flash, view_fov, modify_smoke)
	};
};
