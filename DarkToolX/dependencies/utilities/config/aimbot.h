#pragma once
#include "common.h"

namespace config {
	struct weapon_settings {
		int hitchance{ 1 };
		int min_dmg{ 1 };
		int min_dmg_override{ 1 };
		JSON_SERIALIZE(weapon_settings, hitchance, min_dmg, min_dmg_override)
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
		bool chest{ true };
		bool stomach{ true };
		bool legs{ true };
		bool arms{ false };
		bool auto_shoot{ false };
		bool dmg_indicator{ false };
		bool auto_cock_revolver{ true };
		bool auto_scope{ true };
		int weapon_selection{ 0 };
		std::vector<weapon_settings> weapons{ {20, 1, 1}, {75, 20, 1}, {55, 1, 1}, {60, 10, 1}, {40, 1, 1}, {40, 1, 1}, {50, 1, 1}, {50, 15, 1}, {40, 1, 1}, {55, 1, 1}, {70, 1, 1}, {70, 1, 1}, {90, 100, 1}, {60, 1, 1}, {85, 1, 1}, {60, 1, 1}, {60, 1, 1}, {60, 1, 1}, {85, 1, 1}, {1, 1, 1}, {80, 70, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1} };
		weapon_settings& get_selected() noexcept
		{
			weapons.resize(34);
			return weapons.at(weapon_selection);
		}
		weapon_settings get_weapon_settings(const int weapon) noexcept
		{
			weapons.resize(34);
			const auto i = get_weapon_index(weapon) - 2;
			if (i < 0)
				return {};
			return weapons.at(i);
		}
		constexpr bool is_hitbox_enabled(const int hitbox) const noexcept
		{
			switch (hitbox)
			{
			case hitbox_head:
			case hitbox_neck:
				return head;
			case hitbox_pelvis:
			case hitbox_stomach:
				return stomach;
			case hitbox_lower_chest:
			case hitbox_chest:
			case hitbox_upper_chest:
				return chest;
			case hitbox_right_thigh:
			case hitbox_left_thigh:
			case hitbox_right_calf:
			case hitbox_left_calf:
			case hitbox_right_foot:
			case hitbox_left_foot:
				return legs;
			case hitbox_right_hand:
			case hitbox_left_hand:
			case hitbox_right_upper_arm:
			case hitbox_right_forearm:
			case hitbox_left_upper_arm:
			case hitbox_left_forearm:
				return arms;
			default:
				return false;
			}
		}
		JSON_SERIALIZE(aimbot, conf_name, mode, enabled, key_bind_type, key_bind, min_dmg_override_active, min_dmg_override_key_bind_type, min_dmg_override_key_bind, smoothness, fov, head, chest, stomach, legs, arms, auto_shoot, dmg_indicator, auto_cock_revolver, auto_scope, weapons)
	};
};
