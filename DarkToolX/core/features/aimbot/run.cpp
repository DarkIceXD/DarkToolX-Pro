#include "../features.hpp"

void features::aimbot::run(c_usercmd* cmd)
{
	if (!csgo::cfg.aimbot().mode)
		return;

	csgo::cfg.aimbot().aim.run();
	csgo::cfg.aimbot().min_dmg_override.run();
	csgo::cfg.aimbot().hitbox_override_bind.run();

	if (!csgo::cfg.aimbot().aim.enabled)
		return;

	if (!csgo::local_player)
		return;

	if (!csgo::local_player->can_shoot())
		return;

	const auto weapon = csgo::local_player->active_weapon();
	if (!weapon)
		return;

	if (csgo::cfg.aimbot().auto_cock_revolver && features::util::cock_revolver(weapon))
	{
		cmd->buttons |= in_attack;
		return;
	}

	const auto weapon_data = weapon->get_weapon_data();
	if (!weapon_data)
		return;

	switch (csgo::cfg.aimbot().mode)
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