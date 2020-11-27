#include "features.hpp"

void features::fov(view_setup_t* view) {
	if (!csgo::local_player)
		return;

	if (csgo::local_player->is_scoped())
		return;

	view->fov += csgo::conf->view().view_fov;
}