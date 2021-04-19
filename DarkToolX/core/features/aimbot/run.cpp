#include "../features.hpp"

void features::aimbot::run(c_usercmd* cmd)
{
	if (!csgo::conf->aimbot().mode)
		return;

	csgo::conf->aimbot().aim.run();
	csgo::conf->aimbot().min_dmg_override.run();
	csgo::conf->aimbot().hitbox_override_bind.run();

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