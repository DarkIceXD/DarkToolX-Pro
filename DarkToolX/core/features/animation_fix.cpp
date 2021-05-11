#include "features.hpp"

static float feet_yaw;
static anim_layer layers[13];
static float poses[24];

void features::animation_fix()
{
	if (!csgo::local_player)
		return;

	if (!csgo::local_player->is_alive())
		return;

	const auto animation_state = csgo::local_player->get_anim_state();
	if (!animation_state)
		return;

	if (csgo::should_animate)
	{
		csgo::local_player->get_anim_layers(layers);
		csgo::local_player->update_client_side_animations();
		if (!interfaces::clientstate->choked_commands) {
			feet_yaw = animation_state->goal_feet_yaw;
			csgo::local_player->get_pose_parameters(poses);
		}
		csgo::should_animate = false;
	}
	csgo::local_player->set_anim_layers(layers);
	csgo::local_player->set_pose_parameters(poses);
	csgo::local_player->set_angles({ 0, feet_yaw, 0 });
}