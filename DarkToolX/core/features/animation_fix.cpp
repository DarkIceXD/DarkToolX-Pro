#include "features.hpp"

static float feet_yaw;
static anim_layer layers[13];
static std::array<float, 24> pose_parameters;

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
			pose_parameters = csgo::local_player->get_pose_parameter();
		}
		csgo::should_animate = false;
	}
	/*csgo::local_player->set_anim_layers(layers);*/
	csgo::local_player->get_pose_parameter() = pose_parameters;
	csgo::local_player->set_angles({ 0, feet_yaw, 0 });
}