#pragma once
#include "../../source-sdk/classes/entities.hpp"
#include "../json.hpp"

#define JSON_FROM(v1) \
	if(nlohmann_json_j.contains(#v1)) \
		nlohmann_json_j.at(#v1).get_to(nlohmann_json_t.v1); \

#define JSON_SERIALIZE(Type, ...)  \
    friend void to_json(nlohmann::json& nlohmann_json_j, const Type& nlohmann_json_t) { NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_TO, __VA_ARGS__)) } \
    friend void from_json(const nlohmann::json& nlohmann_json_j, Type& nlohmann_json_t) { NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(JSON_FROM, __VA_ARGS__)) }

namespace config {
	struct keybind {
		bool enabled{ false };
		int type{ 0 };
		int key_bind{ 0 };
		JSON_SERIALIZE(keybind, enabled, type, key_bind)
	};
	constexpr const char* weapon_list_all = "Gloves\0Knife\0CZ75-Auto\0Desert Eagle\0Dual Berettas\0Five-SeveN\0Glock-18\0P2000\0P250\0R8 Revolver\0Tec-9\0USP-S\0AK-47\0AUG\0AWP\0FAMAS\0G3SG1\0Galil AR\0M4A1-S\0M4A4\0SCAR-20\0SG 553\0SSG 08\0MAC-10\0MP5-SD\0MP7\0MP9\0PP-Bizon\0P90\0UMP-45\0Nova\0MAG-7\0Sawed-Off\0XM1014\0M249\0Negev\0";
	constexpr const char* weapon_list = weapon_list_all + 13;
	constexpr int get_weapon_index(const int weapon) noexcept
	{
		switch (weapon)
		{
		case GLOVE_T_SIDE:
			return 0;
		case WEAPON_KNIFE:
		case WEAPON_KNIFE_T:
		case WEAPON_KNIFE_BAYONET:
		case WEAPON_KNIFE_CSS:
		case WEAPON_KNIFE_FLIP:
		case WEAPON_KNIFE_GUT:
		case WEAPON_KNIFE_KARAMBIT:
		case WEAPON_KNIFE_M9_BAYONET:
		case WEAPON_KNIFE_TACTICAL:
		case WEAPON_KNIFE_FALCHION:
		case WEAPON_KNIFE_SURVIVAL_BOWIE:
		case WEAPON_KNIFE_BUTTERFLY:
		case WEAPON_KNIFE_PUSH:
		case WEAPON_KNIFE_CORD:
		case WEAPON_KNIFE_CANIS:
		case WEAPON_KNIFE_URSUS:
		case WEAPON_KNIFE_GYPSY_JACKKNIFE:
		case WEAPON_KNIFE_OUTDOOR:
		case WEAPON_KNIFE_STILETTO:
		case WEAPON_KNIFE_WIDOWMAKER:
		case WEAPON_KNIFE_SKELETON:
			return 1;
		case WEAPON_CZ75A: return 2;
		case WEAPON_DEAGLE: return 3;
		case WEAPON_ELITE: return 4;
		case WEAPON_FIVESEVEN: return 5;
		case WEAPON_GLOCK: return 6;
		case WEAPON_HKP2000: return 7;
		case WEAPON_P250: return 8;
		case WEAPON_REVOLVER: return 9;
		case WEAPON_TEC9: return 10;
		case WEAPON_USP_SILENCER: return 11;
		case WEAPON_AK47: return 12;
		case WEAPON_AUG: return 13;
		case WEAPON_AWP: return 14;
		case WEAPON_FAMAS: return 15;
		case WEAPON_G3SG1: return 16;
		case WEAPON_GALILAR: return 17;
		case WEAPON_M4A1_SILENCER: return 18;
		case WEAPON_M4A1: return 19;
		case WEAPON_SCAR20: return 20;
		case WEAPON_SG556: return 21;
		case WEAPON_SSG08: return 22;
		case WEAPON_MAC10: return 23;
		case WEAPON_MP5_SD: return 24;
		case WEAPON_MP7: return 25;
		case WEAPON_MP9: return 26;
		case WEAPON_BIZON: return 27;
		case WEAPON_P90: return 28;
		case WEAPON_UMP45: return 29;
		case WEAPON_NOVA: return 30;
		case WEAPON_MAG7: return 31;
		case WEAPON_SAWEDOFF: return 32;
		case WEAPON_XM1014: return 33;
		case WEAPON_M249: return 34;
		case WEAPON_NEGEV: return 35;
		default: return -1;
		}
	}
};