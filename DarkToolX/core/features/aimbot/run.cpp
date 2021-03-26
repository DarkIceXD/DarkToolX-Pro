#include "../features.hpp"

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

	if (csgo::conf->aimbot().hitbox_override_key_bind_type == 1 && GetAsyncKeyState(csgo::conf->aimbot().hitbox_override_key_bind) & 1)
		csgo::conf->aimbot().hitbox_override_active = !csgo::conf->aimbot().hitbox_override_active;
	else if (csgo::conf->aimbot().hitbox_override_key_bind_type == 2)
		csgo::conf->aimbot().hitbox_override_active = GetAsyncKeyState(csgo::conf->aimbot().hitbox_override_key_bind);

	if (!csgo::conf->aimbot().enabled)
		return;

	if (!csgo::local_player)
		return;

	if (!csgo::local_player->can_shoot())
		return;

	const auto weapon = csgo::local_player->active_weapon();
	if (!weapon)
		return;

	const auto index = weapon->item_definition_index();
	if (csgo::conf->aimbot().auto_cock_revolver && index == WEAPON_REVOLVER && features::util::cock_revolver(weapon))
	{
		cmd->buttons |= in_attack;
		return;
	}

	const auto weapon_data = weapon->get_weapon_data();
	if (!weapon_data)
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