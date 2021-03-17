#include "../features.hpp"

bool features::util::can_shoot(weapon_t* weapon, const weapon_info_t* weapon_data)
{
	if (csgo::local_player->next_attack() > interfaces::globals->cur_time)
		return false;

	if (weapon->clip1_count() < 1)
		return false;

	if (!weapon_data->weapon_full_auto && weapon->next_primary_attack() > interfaces::globals->cur_time)
		return false;

	return true;
}