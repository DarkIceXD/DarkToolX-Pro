#include "features.hpp"

bool features::hitchance(const vec3_t& view_angles, player_t* player, const int needed_hitchance, weapon_t* weapon, const weapon_info_t* weapon_data)
{
	constexpr auto max_traces = 200;
	const int needed_hits = static_cast<int>(static_cast<float>(max_traces) * needed_hitchance / 100.f);
	const auto start = csgo::local_player->get_eye_pos();
	const auto spreadAngle = math::RAD2DEG(weapon->inaccuracy() + weapon->get_spread());
	int hits = 0;
	for (int i = 0; i < max_traces; i++) {

		// Ratio of the loop (0 at beginning, 1 at end)
		const auto ratio = i / (float)max_traces;

		// This is the spread angle distance
		const auto multiplier = ratio * spreadAngle;

		// Calculate spread direction spiral (30 is a number I chose, you can test other numbers too)
		// Using sqrtf to make our spread circle exponentially distributed
		const auto spreadDir = sqrtf(ratio) * math::M_PI * 30;

		// Now we can calculate our actual spread angle
		const auto spreadAngle = vec3_t(cos(spreadDir) * multiplier, sin(spreadDir) * multiplier, 0);

		// The tested shot angle (spread added to view angles)
		const auto shotAngle = view_angles + spreadAngle;

		// Calculate where this simulated spread shot will end
		const auto endpos = start + math::angle_vector(shotAngle) * weapon_data->weapon_range;

		// An example of doing a trace check on a player
		// If you want good ragebot performance, I wouldn't do this
		ray_t ray(start, endpos);
		trace_t tr;
		interfaces::trace_ray->clip_ray_to_entity(ray, MASK_SHOT, player, &tr);
		if (tr.entity == player)
			hits++;

		if (hits >= needed_hits)
			return true;

		if ((max_traces - i + hits) < needed_hits)
			return false;
	}
	return false;
}