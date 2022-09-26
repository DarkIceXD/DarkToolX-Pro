#include "features.hpp"

void features::crosshair()
{
	if (!csgo::cfg.view().crosshair)
		return;

	if (!csgo::local_player)
		return;

	if (!csgo::local_player->is_alive())
		return;

	if (csgo::cfg.view().show_recoil)
	{
		vec3_t view_angles;
		interfaces::engine->get_view_angles(view_angles);
		view_angles += csgo::local_player->recoil();

		const auto forward = math::angle_vector(view_angles) * 10000;
		const auto end = csgo::local_player->get_eye_pos() + forward;
		vec2_t end_screen;
		if (!math::world_to_screen(end, end_screen))
			return;
		render::draw_line(end_screen.x - 8, end_screen.y, end_screen.x + 8, end_screen.y, color::green());
		render::draw_line(end_screen.x, end_screen.y - 8, end_screen.x, end_screen.y + 8, color::green());
	}
	else
	{
		auto x = 0;
		auto y = 0;
		interfaces::surface->get_screen_size(x, y);
		x /= 2;
		y /= 2;
		render::draw_line(x - 8, y, x + 8, y, color::green());
		render::draw_line(x, y - 8, x, y + 8, color::green());
	}
}