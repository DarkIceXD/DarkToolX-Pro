#include "features.hpp"

void features::auto_shoot(c_usercmd* cmd, weapon_t* weapon, const weapon_info_t* weapon_data)
{
	if (cmd->buttons & in_attack)
		return;

	const auto time = interfaces::globals->interval_per_tick * cmd->tick_count;
	if (csgo::local_player->next_attack() > time)
		return;

	if (!weapon_data->weapon_full_auto && weapon->next_primary_attack() > time)
		return;

	const auto entity = features::auto_wall(math::angle_vector(cmd->viewangles + csgo::local_player->recoil()), weapon_data, false).entity;
	if (!entity)
		return;

	const auto weapon_setting = csgo::conf->aimbot().get_weapon_settings(weapon->item_definition_index());
	if (!features::hitchance(cmd->viewangles, entity, weapon_setting.hitchance, weapon, weapon_data->weapon_range))
		return;
	
	cmd->buttons |= in_attack;
	csgo::want_to_shoot = true;
}

void features::trigger(c_usercmd* cmd)
{
	if (csgo::conf->trigger().key_bind_type == 1 && GetAsyncKeyState(csgo::conf->trigger().key_bind) & 1)
		csgo::conf->trigger().enabled = !csgo::conf->trigger().enabled;
	else if (csgo::conf->trigger().key_bind_type == 2 && !GetAsyncKeyState(csgo::conf->trigger().key_bind))
		return;

	if (!csgo::conf->trigger().enabled)
		return;

	const auto weapon = csgo::local_player->active_weapon();
	if (!weapon)
		return;

	if (weapon->clip1_count() < 1)
		return;

	const auto weapon_data = weapon->get_weapon_data();
	if (!weapon_data)
		return;

	auto_shoot(cmd, weapon, weapon_data);
}