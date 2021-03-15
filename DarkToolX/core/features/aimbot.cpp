#include "features.hpp"

void features::aimbot::run(c_usercmd* cmd)
{
	if (!csgo::conf->aimbot().mode)
		return;

	if (csgo::conf->aimbot().key_bind_type == 1 && GetAsyncKeyState(csgo::conf->aimbot().key_bind) & 1)
		csgo::conf->aimbot().enabled = !csgo::conf->aimbot().enabled;
	else if (csgo::conf->aimbot().key_bind_type == 2 && !GetAsyncKeyState(csgo::conf->aimbot().key_bind))
		return;

	if (csgo::conf->aimbot().min_dmg_override_key_bind_type == 1 && GetAsyncKeyState(csgo::conf->aimbot().min_dmg_override_key_bind) & 1)
		csgo::conf->aimbot().min_dmg_override_active = !csgo::conf->aimbot().min_dmg_override_active;
	else if (csgo::conf->aimbot().min_dmg_override_key_bind_type == 2)
		csgo::conf->aimbot().min_dmg_override_active = GetAsyncKeyState(csgo::conf->aimbot().min_dmg_override_key_bind);

	if (!csgo::conf->aimbot().enabled)
		return;

	if (!csgo::local_player)
		return;

	if (csgo::local_player->next_attack() > interfaces::globals->cur_time)
		return;

	const auto weapon = csgo::local_player->active_weapon();
	if (!weapon)
		return;

	if (weapon->clip1_count() < 1)
		return;

	const auto weapon_data = weapon->get_weapon_data();
	if (!weapon_data)
		return;

	if (!weapon_data->weapon_full_auto && weapon->next_primary_attack() > interfaces::globals->cur_time)
		return;

	switch (csgo::conf->aimbot().mode)
	{
	case 1:
		silent(cmd, weapon, weapon_data);
		break;
	case 2:
		legit(cmd, weapon, weapon_data);
		break;
	default:
		break;
	}
}