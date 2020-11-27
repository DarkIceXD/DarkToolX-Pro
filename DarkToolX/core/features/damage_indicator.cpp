#include "features.hpp"

void features::damage_indicator()
{
	if (csgo::conf->aimbot().mode != 1)
		return;
	
	if (!csgo::conf->aimbot().dmg_indicator)
		return;

	if (!csgo::local_player)
		return;

	if (!csgo::local_player->is_alive())
		return;

	auto text_pos_w = 0;
	auto text_pos_h = 0;
	interfaces::surface->get_screen_size(text_pos_w, text_pos_h);
	text_pos_w /= 2;
	text_pos_h = text_pos_h / 2 + 15;
	render::text(text_pos_w, text_pos_h, render::fonts::watermark_font, std::to_string(csgo::menu::best_dmg), true, color::white());
}