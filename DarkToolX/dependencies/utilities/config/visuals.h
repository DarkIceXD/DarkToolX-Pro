#pragma once
#include "common.h"

namespace config {
	struct rgb {
		float r{ 1 }, g{ 1 }, b{ 1 }, a{ 1 };
		bool enabled{ false };
		constexpr static rgb scale(const rgb& base, const rgb& end, const float c) noexcept
		{
			constexpr auto scale = [](const float max, const float min, const float scale) { return min + (max - min) * scale; };
			return {
				scale(base.r, end.r, c),
				scale(base.g, end.g, c),
				scale(base.b, end.b, c),
				scale(base.a, end.a, c),
				true
			};
		}
		constexpr uint32_t to_u32() const noexcept
		{
			return ((uint32_t)(r * 255) << 0) | ((uint32_t)(g * 255) << 8) | ((uint32_t)(b * 255) << 16) | ((uint32_t)(a * 255) << 24);
		}
		constexpr void rainbow(const float step) noexcept
		{
			if (r > 0 && b == 0)
			{
				r = max(r - step, 0.f);
				g = min(g + step, 1.f);
			}
			else if (g > 0 && r == 0)
			{
				g = max(g - step, 0.f);
				b = min(b + step, 1.f);
			}
			else if (b > 0 && g == 0)
			{
				r = min(r + step, 1.f);
				b = max(b - step, 0.f);
			}
			else
			{
				r = 1;
				g = 0;
				b = 0;
			}
		}
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(rgb, r, g, b, a, enabled)
	};

	struct visuals
	{
		visuals()
		{
			conf_name.reserve(20);
		}
		std::string conf_name{ "New Config" };
		int selection{ 0 };
		std::array<rgb, 11> glow;
		bool health_based{ true };
		bool esp{ false };
		bool dormant_esp{ true };
		bool aimbot_spot{ false };
		rgb box_team{ 0, 0, 1, 1 };
		rgb box_enemy{ 1, 0, 0, 1 };
		rgb skeleton_team{ 1, 1, 1, 1 };
		rgb skeleton_enemy{ 1, 1, 1, 1 };
		rgb step_color{ 0, 0.5f, 1, 1, false };
		float step_max_radius{ 500 };
		float step_lifetime{ 1 };
		float step_width{ 3 };
		bool bullet_trace_local_player{ false };
		rgb bullet_trace_team{ 0, 0.5f, 1, 1, false };
		rgb bullet_trace_enemy{ 1, 0.5f, 0, 1, false };
		float bullet_trace_lifetime{ 1 };
		constexpr int rgb_index(const int class_id, const bool enemy = false) const noexcept
		{
			switch (class_id)
			{
			case ccsplayer:
				return enemy ? 0 : 1;
			case cc4:
			case cplantedc4:
				return 2;
			case cdeagle:
			case cweaponelite:
			case cweaponfiveseven:
			case cweaponglock:
			case cweaponhkp2000:
			case cweaponp250:
			case cweapontec9:
				return 3;
			case cweaponbizon:
			case cweaponmac10:
			case cweaponmp7:
			case cweaponmp9:
			case cweaponp90:
			case cweaponump45:
				return 4;
			case cak47:
			case cweaponaug:
			case cweaponfamas:
			case cweapongalilar:
			case cweaponm4a1:
			case cweaponsg556:
				return 5;
			case cweaponmag7:
			case cweaponnova:
			case cweaponxm1014:
			case cweaponsawedoff:
				return 6;
			case cweaponawp:
			case cweapong3sg1:
			case cweaponscar20:
			case cweaponssg08:
				return 7;
			case cweaponm249:
			case cweaponnegev:
				return 8;
			case cflashbang:
			case cdecoyprojectile:
			case csmokegrenade:
			case csmokegrenadeprojectile:
			case cbasecsgrenadeprojectile:
			case chegrenade:
			case cincendiarygrenade:
			case citem_healthshot:
			case cmolotovgrenade:
			case cmolotovprojectile:
			case ceconentity:
				return 9;
			default:
				return -1;
			}
		}
		constexpr rgb* get_rgb(const int class_id, const bool enemy = false) noexcept
		{
			const auto i = rgb_index(class_id, enemy);
			if (i < 0)
				return nullptr;
			return &glow.at(i);
		}
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(visuals, conf_name, glow, health_based, esp, dormant_esp, aimbot_spot, box_team, box_enemy, skeleton_team, skeleton_enemy, step_color, step_max_radius, step_lifetime, step_width, bullet_trace_local_player, bullet_trace_team, bullet_trace_enemy, bullet_trace_lifetime)
	};
};
