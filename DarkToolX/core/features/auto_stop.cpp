#include "features.hpp"

void features::auto_stop(float& forwardmove, float& sidemove)
{
	if (!csgo::conf->misc().auto_stop)
		return;

	if (csgo::want_to_shoot)
		return;

	if (!csgo::conf->aimbot().auto_shoot)
		return;

	if (!csgo::local_player)
		return;

	const int move_type = csgo::local_player->move_type();
	if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer)
		return;

	if (!(csgo::local_player->flags() & fl_onground))
		return;

	const auto weapon = csgo::local_player->active_weapon();
	if (!weapon)
		return;

	const auto weapon_setting = csgo::conf->aimbot().get_weapon_settings(weapon->item_definition_index());

	switch (csgo::conf->aimbot().mode)
	{
	case 1:
		if (csgo::target.lethal || csgo::target.damage >= (csgo::conf->aimbot().min_dmg_override_active ? weapon_setting.min_dmg_override : weapon_setting.min_dmg))
			forwardmove = sidemove = 0;
		break;
	case 2:
		if (csgo::target.damage)
			forwardmove = sidemove = 0;
		break;
	default:
		break;
	}
}