#include "../features.hpp"

void features::util::auto_stop(c_usercmd* cmd)
{
	if (!csgo::conf->misc().auto_stop)
		return;

	if (!csgo::local_player)
		return;

	const int move_type = csgo::local_player->move_type();
	if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer)
		return;

	if (!(csgo::local_player->flags() & fl_onground))
		return;

	const auto velocity = csgo::local_player->velocity();
	auto direction = math::vector_angles(velocity);
	direction.y = cmd->viewangles.y - direction.y;
	const auto negated_direction = math::angle_vector(direction) * velocity.length();
	cmd->forwardmove = negated_direction.x;
	cmd->sidemove = negated_direction.y;
}