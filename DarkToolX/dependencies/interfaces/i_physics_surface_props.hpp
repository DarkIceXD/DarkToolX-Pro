#pragma once

struct surfacedata_t
{
	unsigned char pad0[80];
	float maxSpeedFactor;
	float jumpFactor;
	float penetrationModifier;
	float damageModifier;
	unsigned short material;
	unsigned char climbable;
};

class i_physics_surface_props {
public:
	surfacedata_t* get_surface_data(int index) {
		using original_fn = surfacedata_t * (__thiscall*)(i_physics_surface_props*, int);
		return (*(original_fn**)this)[5](this, index);
	}
};