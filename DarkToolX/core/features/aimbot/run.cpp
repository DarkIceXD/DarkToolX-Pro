#include "../features.hpp"

void features::aimbot::run(c_usercmd* cmd)
{
	if (!csgo::conf->aimbot().mode)
		return;

	if (csgo::conf->aimbot().aim.type == 1 && GetAsyncKeyState(csgo::conf->aimbot().aim.key_bind) & 1)
		csgo::conf->aimbot().aim.enabled = !csgo::conf->aimbot().aim.enabled;
	else if (csgo::conf->aimbot().aim.type == 2)
		csgo::conf->aimbot().aim.enabled = GetAsyncKeyState(csgo::conf->aimbot().aim.key_bind);

	if (csgo::conf->aimbot().min_dmg_override.type == 1 && GetAsyncKeyState(csgo::conf->aimbot().min_dmg_override.key_bind) & 1)
		csgo::conf->aimbot().min_dmg_override.enabled = !csgo::conf->aimbot().min_dmg_override.enabled;
	else if (csgo::conf->aimbot().min_dmg_override.type == 2)
		csgo::conf->aimbot().min_dmg_override.enabled = GetAsyncKeyState(csgo::conf->aimbot().min_dmg_override.key_bind);

	if (csgo::conf->aimbot().hitbox_override_bind.type == 1 && GetAsyncKeyState(csgo::conf->aimbot().hitbox_override_bind.key_bind) & 1)
		csgo::conf->aimbot().hitbox_override_bind.enabled = !csgo::conf->aimbot().hitbox_override_bind.enabled;
	else if (csgo::conf->aimbot().hitbox_override_bind.type == 2)
		csgo::conf->aimbot().hitbox_override_bind.enabled = GetAsyncKeyState(csgo::conf->aimbot().hitbox_override_bind.key_bind);

	if (!csgo::conf->aimbot().aim.enabled)
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