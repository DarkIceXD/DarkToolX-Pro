#pragma once
#include <thread>
#include "fnv.hpp"
#include "utilities.hpp"
#include "../interfaces/interfaces.hpp"
#include "../../source-sdk/sdk.hpp"
#include "../../core/hooks/hooks.hpp"
#include "../math/math.hpp"
#include "renderer/renderer.hpp"
#include "console/console.hpp"
#include "License.h"
#include "config/config.h"
#include "kit_parser.h"

//interfaces
#define sig_client_state "A1 ? ? ? ? 8B 80 ? ? ? ? C3"
#define sig_directx "A1 ? ? ? ? 50 8B 08 FF 51 0C"
#define sig_input "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10"
#define sig_glow_manager "0F 11 05 ? ? ? ? 83 C8 01 C7 05 ? ? ? ? 00 00 00 00"
#define sig_player_move_helper "8B 0D ? ? ? ? 8B 46 08 68"
#define sig_weapon_data "8B 35 ? ? ? ? FF 10 0F B7 C0"

//misc
#define sig_set_angles "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"
#define sig_prediction_random_seed "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04"

namespace csgo {
	inline player_t* local_player = nullptr;
	inline conf* conf = nullptr;
	inline kit_parser* kits = nullptr;
	inline bool want_to_shoot = false;
	inline License::User user;
	namespace menu {
		inline bool enabled = false;
		inline int best_dmg = 0;
	};
}