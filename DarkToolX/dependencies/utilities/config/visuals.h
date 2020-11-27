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
		JSON_SERIALIZE(rgb, r, g, b, a, enabled)
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
		bool enemy_overhead_info{ true };
		rgb step_color{ 0, 0.5f, 1, 1, false };
		float step_max_radius{ 500 };
		float step_lifetime{ 1 };
		float step_width{ 3 };
		constexpr int rgb_index(const int class_id, const bool enemy = false) const noexcept
		{
			switch (class_id)
			{
			case ccsplayer:
				if (enemy)
					return 0;
				else
					return 1;
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
		JSON_SERIALIZE(visuals, conf_name, glow, health_based, enemy_overhead_info, step_color, step_max_radius, step_lifetime, step_width)
	};
};
