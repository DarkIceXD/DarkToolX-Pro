#include "features.hpp"

void features::trigger(c_usercmd* cmd)
{
	if (csgo::conf->trigger().bind.type == 1 && GetAsyncKeyState(csgo::conf->trigger().bind.key_bind) & 1)
		csgo::conf->trigger().bind.enabled = !csgo::conf->trigger().bind.enabled;
	else if (csgo::conf->trigger().bind.type == 2)
		csgo::conf->trigger().bind.enabled = GetAsyncKeyState(csgo::conf->trigger().bind.key_bind);

	if (!csgo::conf->trigger().bind.enabled)
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