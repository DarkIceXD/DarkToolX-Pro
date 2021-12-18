#include "../features.hpp"

void features::util::auto_shoot(c_usercmd* cmd, weapon_t* weapon, const weapon_info_t* weapon_data)
{
	if (cmd->buttons & in_attack)
		return;

	const auto entity = auto_wall(csgo::local_player->get_eye_pos(), math::angle_vector(cmd->viewangles + csgo::local_player->recoil()), weapon_data, csgo::conf->aimbot().auto_wall).entity;
	if (!entity)
		return;

	const auto weapon_setting = csgo::conf->aimbot().get_weapon_settings(weapon->item_definition_index());
	if (!hitchance(cmd->viewangles, entity, weapon_setting.hitchance, weapon, weapon_data->range))
	{
		if (csgo::conf->aimbot().auto_scope && weapon_data->type == WEAPONTYPE_SNIPER_RIFLE && !csgo::local_player->is_scoped())
			cmd->buttons |= in_attack2;
		csgo::didnt_shoot_due_to_hitchance = true;
		return;
	}

	cmd->buttons |= in_attack;
	csgo::want_to_shoot = true;
}