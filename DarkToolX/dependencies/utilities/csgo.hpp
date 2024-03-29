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
#include "config/config.h"
#include "license.h"

struct best_target {
	player_t* entity;
	vec3_t angle;
	vec3_t aim_spot;
	int damage;
	float fov;
	int best_record;
	bool lethal;
};

constexpr bool sort_by_dmg(const best_target& a, const best_target& b)
{
	return a.lethal != b.lethal ? a.lethal : a.damage > b.damage;
}

namespace csgo {
	inline license::result user;
	inline player_t* local_player = nullptr;
	inline conf cfg;
	inline bool want_to_shoot = false;
	inline bool manual_shoot = false;
	inline bool didnt_shoot_due_to_hitchance = false;
	inline bool should_animate = true;
	inline vec3_t real = {};
	inline best_target target = {};
	namespace menu {
		inline bool enabled = false;
	};
}