#include "features.hpp"

void features::trigger(c_usercmd* cmd)
{
	if (csgo::conf->trigger().key_bind_type == 1 && GetAsyncKeyState(csgo::conf->trigger().key_bind) & 1)
		csgo::conf->trigger().enabled = !csgo::conf->trigger().enabled;
	else if (csgo::conf->trigger().key_bind_type == 2 && !GetAsyncKeyState(csgo::conf->trigger().key_bind))
		return;

	if (!csgo::conf->trigger().enabled)
		return;

	if (!csgo::local_player->can_shoot())
		return;

	const auto weapon = csgo::local_player->active_weapon();
	if (!weapon)
		return;

	const auto weapon_data = weapon->get_weapon_data();
	if (!weapon_data)
		return;

	features::util::auto_shoot(cmd, weapon, weapon_data);
}