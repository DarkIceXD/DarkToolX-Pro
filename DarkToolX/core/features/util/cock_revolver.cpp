#include "../features.hpp"

bool features::util::cock_revolver(weapon_t* weapon)
{
	if (weapon->item_definition_index() == WEAPON_REVOLVER && weapon->postpone_fire_ready() - 1.f >= interfaces::globals->cur_time)
		return true;
	return false;
}