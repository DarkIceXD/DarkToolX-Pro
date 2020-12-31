#include "features.hpp"

static weapon_t* get_weapon_of_type(const int type)
{
	weapon_t* weapon = nullptr;
	auto weapons = csgo::local_player->get_weapons();
	for (auto i = 0; i < 64; i++)
	{
		weapon = (weapon_t*)interfaces::entity_list->get_client_entity_handle(weapons[i]);
		if (!weapon)
			break;

		const auto weapon_data = weapon->get_weapon_data();
		if (!weapon_data)
			continue;

		if (weapon_data->weapon_type == type)
			break;
	}
	return weapon;
}

static weapon_t* get_weapon(const int type)
{
	weapon_t* weapon = nullptr;
	auto weapons = csgo::local_player->get_weapons();
	for (auto i = 0; i < 64; i++)
	{
		weapon = (weapon_t*)interfaces::entity_list->get_client_entity_handle(weapons[i]);
		if (!weapon)
			break;

		if (weapon->item_definition_index() == type)
			break;
	}
	return weapon;
}

void features::auto_switch(c_usercmd* cmd) {
	static auto switch_phase = 0;

	if (!csgo::conf->misc().auto_switch)
		return;

	if (!csgo::local_player)
		return;

	const auto weapon = csgo::local_player->active_weapon();
	if (!weapon)
	{
		switch_phase = 0;
		return;
	}

	if (switch_phase == 1)
	{
		auto zeus = get_weapon(WEAPON_TASER);
		if (zeus)
			cmd->weaponselect = zeus->index();
		else
		{
			auto knife = get_weapon_of_type(WEAPONTYPE_KNIFE);
			if (knife)
				cmd->weaponselect = knife->index();
		}
		switch_phase = 2;
	}
	else if (switch_phase == 2)
	{
		auto sniper = get_weapon_of_type(WEAPONTYPE_SNIPER_RIFLE);
		if (sniper)
			cmd->weaponselect = sniper->index();
		cmd->buttons |= in_attack2;
		switch_phase = 3;
	}
	else if (switch_phase == 3)
	{
		const auto time = interfaces::globals->interval_per_tick * cmd->tick_count;
		if (weapon->next_primary_attack() <= time)
		{
			cmd->buttons |= in_attack2;
			switch_phase = 0;
		}
	}
	else
	{
		if (!(cmd->buttons & in_attack))
			return;

		if (!csgo::local_player->is_scoped())
			return;

		const auto index = weapon->item_definition_index();
		if (index == WEAPON_SSG08 || index == WEAPON_AWP)
			switch_phase = 1;
	}
}