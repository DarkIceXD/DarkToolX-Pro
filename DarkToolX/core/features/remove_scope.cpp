#include "features.hpp"

bool features::remove_scope()
{
	if (!csgo::conf->view().remove_scope)
		return false;

	if (!csgo::local_player)
		return false;

	if (!csgo::local_player->is_scoped())
		return false;

	const auto weapon = csgo::local_player->active_weapon();
	if (!weapon)
		return false;

	const auto weapon_data = weapon->get_weapon_data();
	if (!weapon_data)
		return false;

	const auto type = weapon_data->type;
	if (type != WEAPONTYPE_SNIPER_RIFLE)
		return false;

	auto x = 0;
	auto y = 0;
	interfaces::surface->get_screen_size(x, y);
	render::draw_line(0, y / 2, x, y / 2, color::black());
	render::draw_line(x / 2, 0, x / 2, y, color::black());
	static auto postprocess = interfaces::console->get_convar("mat_postprocess_enable");
	postprocess->set_value(0);
	return true;
}