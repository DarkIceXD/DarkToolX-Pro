#pragma once
#include "common.h"

namespace config {
	struct skin_animation_frame
	{
		skin_animation_frame()
		{
			nametag.reserve(20);
		}
		skin_animation_frame(const int paint_kit, const int stat_trak, const int seed, const float wear, const char* nametag)
			: paint_kit(paint_kit), stat_trak(stat_trak), seed(seed), wear(wear), nametag(nametag)
		{
			this->nametag.reserve(20);
		}
		int paint_kit{ 0 }, stat_trak{ 0 }, seed{ 0 };
		float wear{ FLT_MIN };
		std::string nametag;
		JSON_SERIALIZE(skin_animation_frame, paint_kit, stat_trak, seed, wear, nametag)
	};

	struct skin {
		skin()
		{
			nametag.reserve(20);
		}
		int paint_kit{ 0 }, stat_trak{ 0 }, seed{ 0 }, quality{ 0 };
		float wear{ FLT_MIN };
		std::string nametag;
		size_t frame{ 0 };
		std::vector<skin_animation_frame> animation;
		bool count_kills{ true };
		bool animated_skins{ false };
		bool reverse{ true };
		bool go_forward{ true };
		int delay_ticks{ 2 };
		constexpr skin_animation_frame* next_animation() noexcept
		{
			if (!animated_skins)
				return nullptr;
			const auto size = animation.size();
			if (size == 0)
				return nullptr;
			if (frame == 0)
				go_forward = true;
			else if (frame >= size - 1)
				if (reverse)
					go_forward = false;
				else
				{
					go_forward = true;
					frame = 0;
				}
			frame += go_forward ? 1 : -1;
			if (frame >= size)
				frame = 0;
			return &animation.at(frame);
		}
		JSON_SERIALIZE(skin, paint_kit, stat_trak, seed, quality, wear, nametag, animation, count_kills, animated_skins, reverse, delay_ticks)
	};

	struct skin_changer
	{
		skin_changer()
		{
			conf_name.reserve(20);
		}
		std::string conf_name{ "New Config" };
		bool enabled{ true };
		bool auto_save{ true };
		int selection{ 0 };
		std::array<skin, 36> skins;
		int knife_selection{ 0 };
		int glove_selection{ 0 };
		constexpr skin* get_skin(const int weapon) noexcept
		{
			const auto i = get_weapon_index(weapon);
			if (i < 0)
				return nullptr;
			return &skins.at(i);
		};
		constexpr skin& get_selected() noexcept
		{
			return skins.at(selection);
		}
		constexpr short get_knife_index() const noexcept
		{
			switch (knife_selection)
			{
			case 1: return WEAPON_KNIFE_BAYONET;
			case 2: return WEAPON_KNIFE_CSS;
			case 3: return WEAPON_KNIFE_FLIP;
			case 4: return WEAPON_KNIFE_GUT;
			case 5: return WEAPON_KNIFE_KARAMBIT;
			case 6: return WEAPON_KNIFE_M9_BAYONET;
			case 7: return WEAPON_KNIFE_TACTICAL;
			case 8: return WEAPON_KNIFE_FALCHION;
			case 9: return WEAPON_KNIFE_SURVIVAL_BOWIE;
			case 10: return WEAPON_KNIFE_BUTTERFLY;
			case 11: return WEAPON_KNIFE_PUSH;
			case 12: return WEAPON_KNIFE_CORD;
			case 13: return WEAPON_KNIFE_CANIS;
			case 14: return WEAPON_KNIFE_URSUS;
			case 15: return WEAPON_KNIFE_GYPSY_JACKKNIFE;
			case 16: return WEAPON_KNIFE_OUTDOOR;
			case 17: return WEAPON_KNIFE_STILETTO;
			case 18: return WEAPON_KNIFE_WIDOWMAKER;
			case 19: return WEAPON_KNIFE_SKELETON;
			default: return 0;
			}
		};
		constexpr short get_glove_index() const noexcept
		{
			switch (glove_selection)
			{
			case 1: return GLOVE_STUDDED_BLOODHOUND;
			case 2: return GLOVE_SPORTY;
			case 3: return GLOVE_SLICK;
			case 4: return GLOVE_LEATHER_WRAP;
			case 5: return GLOVE_MOTORCYCLE;
			case 6: return GLOVE_SPECIALIST;
			case 7: return GLOVE_HYDRA;
			case 8: return GLOVE_BROKEN_FANG;
			default: return 0;
			}
		};
		JSON_SERIALIZE(skin_changer, conf_name, enabled, auto_save, skins, knife_selection, glove_selection)
	};
};