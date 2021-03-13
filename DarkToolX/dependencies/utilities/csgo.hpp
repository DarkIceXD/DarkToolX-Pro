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

struct best_target {
	player_t* entity;
	vec3_t angle;
	int damage;
	int hitbox;
	float fov;
	bool lethal;
};

constexpr bool sort_by_dmg(const best_target& a, const best_target& b)
{
	return a.lethal != b.lethal ? a.lethal : a.damage > b.damage;
}

namespace csgo {
	inline player_t* local_player = nullptr;
	inline conf* conf = nullptr;
	inline bool want_to_shoot = false;
	inline bool manual_shoot = false;
	inline License::User user;
	inline bool should_animate = false;
	inline vec3_t real = {};
	inline best_target target = {};
	namespace menu {
		inline bool enabled = false;
	};
}