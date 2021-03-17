#include "../features.hpp"

/* old hitchance
int features::hitbox_to_hitgroup(const int hitbox)
{
	switch (hitbox)
	{
	case hitbox_head:
	case hitbox_neck:
		return hitgroup_head;
	case hitbox_pelvis:
	case hitbox_stomach:
		return hitgroup_stomach;
	case hitbox_lower_chest:
	case hitbox_chest:
	case hitbox_upper_chest:
		return hitgroup_chest;
	case hitbox_right_thigh:
	case hitbox_right_calf:
	case hitbox_right_foot:
		return hitgroup_rightleg;
	case hitbox_left_thigh:
	case hitbox_left_calf:
	case hitbox_left_foot:
		return hitgroup_leftleg;
	case hitbox_right_hand:
	case hitbox_right_upper_arm:
	case hitbox_right_forearm:
		return hitgroup_rightarm;
	case hitbox_left_hand:
	case hitbox_left_upper_arm:
	case hitbox_left_forearm:
		return hitgroup_leftarm;
	default:
		return hitgroup_generic;
	}
}

static bool hitgroup_is_allowed(const int wanted_hitgroup, const int hitted_hitgroup)
{
	switch (wanted_hitgroup)
	{
	case hitgroup_head:
		return hitted_hitgroup == hitgroup_head;
	case hitgroup_stomach:
		return hitted_hitgroup == hitgroup_stomach;
	default:
		return true;
	}
}

bool features::hitchance(const vec3_t& view_angles, player_t* player, const int needed_hitchance, weapon_t* weapon, const weapon_info_t* weapon_data, const int hitgroup)
{
	constexpr auto max_traces = 200;
	const int needed_hits = static_cast<int>(static_cast<float>(max_traces) * needed_hitchance / 100.f);
	const auto start = csgo::local_player->get_eye_pos();
	const auto spreadAngle = math::RAD2DEG(weapon->inaccuracy() + weapon->get_spread());
	int hits = 0;
	for (int i = 0; i < max_traces; i++)
	{
		const auto ratio = i / (float)max_traces;
		const auto multiplier = ratio * spreadAngle;
		const auto spreadDir = sqrtf(ratio) * math::M_PI * 30;
		const auto spreadAngle = vec3_t(cos(spreadDir) * multiplier, sin(spreadDir) * multiplier, 0);
		const auto shotAngle = view_angles + spreadAngle;
		const auto endpos = start + math::angle_vector(shotAngle) * weapon_data->weapon_range;

		ray_t ray(start, endpos);
		trace_t tr;
		interfaces::trace_ray->clip_ray_to_entity(ray, MASK_SHOT, player, &tr);
		if (tr.entity == player)
		{
			if (hitgroup)
			{
				if (hitgroup_is_allowed(hitgroup, tr.hitGroup))
					hits++;
			}
			else
			{
				hits++;
			}
		}

		if (hits >= needed_hits)
			return true;

		if ((max_traces - i + hits) < needed_hits)
			return false;
	}
	return false;
}*/

static float RandomFloat(float min, float max)
{
	typedef float(*fn)(float, float);
	static fn rand_float = (fn)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat");
	return rand_float(min, max);
}

static void RandomSeed(unsigned int seed)
{
	typedef void(*fn)(unsigned int);
	static fn rand_seed = (fn)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomSeed");
	rand_seed(seed);
}

bool features::util::hitchance(const vec3_t& view_angles, player_t* player, const int needed_hitchance, weapon_t* weapon, const float range)
{
	constexpr auto seeds = 256;
	const int needed_hits = static_cast<int>(static_cast<float>(seeds) * needed_hitchance / 100.f);
	const auto start = csgo::local_player->get_eye_pos();
	const auto inaccuracy = weapon->inaccuracy();
	const auto spread = weapon->get_spread();
	int hits = 0;
	vec3_t forward, right, up;
	math::angle_vectors(view_angles + csgo::local_player->recoil(), &forward, &right, &up);
	for (int i = 0; i < seeds; i++)
	{
		RandomSeed(i);
		const auto rand_inaccuracy = RandomFloat(0.f, 1.f) * inaccuracy;
		const auto rand_angle_1 = RandomFloat(0.f, (math::M_PI * 2));
		const auto rand_spread = RandomFloat(0.f, 1.f) * spread;
		const auto rand_angle_2 = RandomFloat(0.f, (math::M_PI * 2));
		const auto spread_offset_x = cos(rand_angle_1) * rand_inaccuracy + cos(rand_angle_2) * rand_spread;
		const auto spread_offset_y = sin(rand_angle_1) * rand_inaccuracy + sin(rand_angle_2) * rand_spread;
		auto spread_forward = forward + right * spread_offset_x + up * spread_offset_y;

		const auto spread_view_angles = math::vector_angles(spread_forward, up).normalized_angles();
		
		const auto end_pos = start + math::angle_vector(spread_view_angles) * range;
		ray_t ray(start, end_pos);
		trace_t tr;
		interfaces::trace_ray->clip_ray_to_entity(ray, MASK_SHOT, player, &tr);
		if (tr.entity == player)
			hits++;

		if (hits >= needed_hits)
			return true;

		if ((seeds - i + hits) < needed_hits)
			return false;
	}
	return false;
}