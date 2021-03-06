#include "features.hpp"

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
				if (tr.hitGroup == hitgroup)
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
}