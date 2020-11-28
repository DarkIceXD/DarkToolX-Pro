#include "features.hpp"

static vec3_t rotation;
static anim_layer layers[13];
static float poses[24];

void features::animation_fix()
{
	if (!csgo::local_player)
		return;

	const auto animation_state = csgo::local_player->get_anim_state();

	if (!animation_state)
		return;

	if (csgo::should_animate) {
		// get real layers
		csgo::local_player->get_anim_layers(layers);

		// update animations
		csgo::local_player->update_client_side_animations();

		// the choke cycle has reset
		if (!interfaces::clientstate->choked_commands) {
			rotation.y = animation_state->goal_feet_yaw;
			csgo::local_player->get_pose_parameters(poses);
		}
		csgo::should_animate = false;
	}

	// update layers, poses, and rotation
	csgo::local_player->set_anim_layers(layers);
	csgo::local_player->set_pose_parameters(poses);
	csgo::local_player->set_angles(rotation);
}