#include "../features.hpp"

bool features::util::cock_revolver(weapon_t* weapon)
{
	if (weapon->postpone_fire_ready() - 1.f >= interfaces::globals->cur_time)
		return true;
	return false;
}