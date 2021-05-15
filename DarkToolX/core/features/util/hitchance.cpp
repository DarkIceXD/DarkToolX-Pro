#include "../features.hpp"

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

static int hitbox_to_hitgroup(const int hitbox)
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

bool features::util::hitchance(const vec3_t& view_angles, player_t* player, const int needed_hitchance, weapon_t* weapon, const float range, const int hitbox)
{
	constexpr auto seeds = 256;
	const int needed_hits = static_cast<int>(static_cast<float>(seeds) * needed_hitchance / 100.f);
	const auto start = csgo::local_player->get_eye_pos();
	weapon->update_accuracy_penalty();
	const auto inaccuracy = weapon->inaccuracy();
	const auto spread = weapon->get_spread();
	int hits = 0;
	vec3_t forward, right, up;
	math::angle_vector(view_angles + csgo::local_player->recoil(), &forward, &right, &up);
	const auto hitgroup = hitbox_to_hitgroup(hitbox);
	for (int i = 0; i < seeds; i++)
	{
		RandomSeed(i);
		const auto rand_inaccuracy = RandomFloat(0.f, 1.f) * inaccuracy;
		const auto rand_angle_1 = RandomFloat(0.f, (math::M_PI * 2));
		const auto rand_spread = RandomFloat(0.f, 1.f) * spread;
		const auto rand_angle_2 = RandomFloat(0.f, (math::M_PI * 2));
		const auto spread_offset_x = cos(rand_angle_1) * rand_inaccuracy + cos(rand_angle_2) * rand_spread;
		const auto spread_offset_y = sin(rand_angle_1) * rand_inaccuracy + sin(rand_angle_2) * rand_spread;
		const auto spread_forward = forward + right * spread_offset_x + up * spread_offset_y;
		const auto spread_view_angles = math::vector_angles(spread_forward, up).normalized_angles();

		const auto end_pos = start + math::angle_vector(spread_view_angles) * range;
		ray_t ray(start, end_pos);
		trace_t tr;
		interfaces::trace_ray->clip_ray_to_entity(ray, MASK_SHOT, player, &tr);
		if (tr.entity == player)
			if (hitbox)
			{
				if (tr.hitGroup == hitgroup)
					hits++;
			}
			else
			{
				hits++;
			}

		if (hits >= needed_hits)
			return true;

		if ((seeds - i + hits) < needed_hits)
			return false;
	}
	return false;
}