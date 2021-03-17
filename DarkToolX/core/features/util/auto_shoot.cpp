#include "../features.hpp"

void features::util::auto_shoot(c_usercmd* cmd, weapon_t* weapon, const weapon_info_t* weapon_data)
{
	if (cmd->buttons & in_attack)
		return;

	const auto entity = auto_wall(math::angle_vector(cmd->viewangles + csgo::local_player->recoil()), weapon_data, false).entity;
	if (!entity)
		return;

	const auto weapon_setting = csgo::conf->aimbot().get_weapon_settings(weapon->item_definition_index());
	if (!hitchance(cmd->viewangles, entity, weapon_setting.hitchance, weapon, weapon_data->weapon_range))
		return;

	cmd->buttons |= in_attack;
	csgo::want_to_shoot = true;
}