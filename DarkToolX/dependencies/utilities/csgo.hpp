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

namespace csgo {
	inline player_t* local_player = nullptr;
	inline conf* conf = nullptr;
	inline bool want_to_shoot = false;
	inline License::User user;
	inline bool should_animate = false;
	inline vec3_t real = {};
	namespace menu {
		inline bool enabled = false;
		inline int best_dmg = 0;
	};
}