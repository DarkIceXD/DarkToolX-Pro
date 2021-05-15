#pragma once
#include "common.h"

namespace config {
	struct weapon_settings {
		int hitchance{ 1 };
		int min_dmg{ 1 };
		int min_dmg_override{ 1 };
		constexpr int get_min_dmg(const bool min_dmg_override_active) const noexcept
		{
			return min_dmg_override_active ? min_dmg_override : min_dmg;
		}
		JSON_SERIALIZE(weapon_settings, hitchance, min_dmg, min_dmg_override)
	};

	struct hitboxes {
		bool head{ true };
		bool chest{ true };
		bool stomach{ true };
		bool legs{ true };
		bool arms{ false };
		JSON_SERIALIZE(hitboxes, head, chest, stomach, legs, arms)
	};

	struct aimbot
	{
		aimbot()
		{
			conf_name.reserve(20);
		}
		std::string conf_name{ "New Config" };
		int mode{ 0 };
		keybind aim{ true, 0, 0x12 };
		keybind min_dmg_override{};
		keybind hitbox_override_bind{};
		float smoothness{ 3.f };
		float fov{ 0.f };
		hitboxes hitbox{};
		hitboxes hitbox_override{};
		bool auto_shoot{ false };
		bool auto_wall{ false };
		bool auto_duck_peek{ false };
		bool auto_cock_revolver{ true };
		bool auto_scope{ true };
		bool dmg_indicator{ false };
		int weapon_selection{ 0 };
		std::vector<weapon_settings> weapons{ {20, 1, 1}, {75, 20, 1}, {55, 1, 1}, {60, 10, 1}, {40, 1, 1}, {40, 1, 1}, {50, 1, 1}, {50, 15, 1}, {40, 1, 1}, {55, 1, 1}, {70, 1, 1}, {70, 1, 1}, {90, 100, 1}, {60, 1, 1}, {85, 1, 1}, {60, 1, 1}, {60, 1, 1}, {60, 1, 1}, {85, 1, 1}, {1, 1, 1}, {80, 70, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1} };
		bool backtrack{ false };
		int backtrack_time_limit{ 200 };
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
			const auto& h = hitbox_override_bind.enabled ? hitbox_override : this->hitbox;
			switch (hitbox)
			{
			case hitbox_head:
			case hitbox_neck:
				return h.head;
			case hitbox_pelvis:
			case hitbox_stomach:
				return h.stomach;
			case hitbox_lower_chest:
			case hitbox_chest:
			case hitbox_upper_chest:
				return h.chest;
			case hitbox_right_thigh:
			case hitbox_left_thigh:
			case hitbox_right_calf:
			case hitbox_left_calf:
			case hitbox_right_foot:
			case hitbox_left_foot:
				return h.legs;
			case hitbox_right_hand:
			case hitbox_left_hand:
			case hitbox_right_upper_arm:
			case hitbox_right_forearm:
			case hitbox_left_upper_arm:
			case hitbox_left_forearm:
				return h.arms;
			default:
				return false;
			}
		}
		JSON_SERIALIZE(aimbot, conf_name, mode, aim, min_dmg_override, hitbox_override_bind, smoothness, fov, hitbox, hitbox_override, auto_shoot, auto_wall, auto_duck_peek, dmg_indicator, auto_cock_revolver, auto_scope, weapons, backtrack, backtrack_time_limit)
	};
};
