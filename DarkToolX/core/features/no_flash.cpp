#include "features.hpp"

void features::no_flash()
{
	if (!csgo::local_player)
		return;

	csgo::local_player->flash_alpha() = csgo::cfg.view().no_flash ? 0.f : 255.f;
}