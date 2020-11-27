#pragma once
#include "common.h"

namespace config {
	struct weapon_settings {
		int min_dmg{ 1 };
		int min_dmg_override{ 1 };
		int hitchance{ 1 };
		JSON_SERIALIZE(weapon_settings, min_dmg, min_dmg_override, hitchance)
	};

	struct aimbot
	{
		aimbot()
		{
			conf_name.reserve(20);
		}
		std::string conf_name{ "New Config" };
		int mode{ 0 };
		bool enabled{ true };
		int key_bind_type{ 0 };
		int key_bind{ 0x12 };
		bool min_dmg_override_active{ false };
		int min_dmg_override_key_bind_type{ 0 };
		int min_dmg_override_key_bind{ 0 };
		float smoothness{ 3.f };
		float fov{ 0.f };
		bool head{ true };
		bool auto_shoot{ false };
		bool dmg_indicator{ false };
		int weapon_selection{ 0 };
		std::vector<weapon_settings> weapons{ 0 };
		weapon_settings& get_selected() noexcept
		{
			weapons.resize(34);
			return weapons.at(weapon_selection);
		}
		weapon_settings* get_weapon_settings(const int weapon) noexcept
		{
			weapons.resize(34);
			const auto i = get_weapon_index(weapon) - 2;
			if (i < 0)
				return nullptr;
			return &weapons.at(i);
		}
		JSON_SERIALIZE(aimbot, conf_name, mode, enabled, key_bind_type, key_bind, min_dmg_override_active, min_dmg_override_key_bind_type, min_dmg_override_key_bind, smoothness, fov, head, auto_shoot, dmg_indicator, weapons)
	};
};
