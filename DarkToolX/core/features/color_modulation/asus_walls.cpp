#include "../features.hpp"

void features::color_modulation::asus_walls::world(i_material* mat)
{
	if (!csgo::conf->view().asus_walls)
		return;

	mat->alpha_modulate(csgo::conf->view().asus_walls_alpha);
}