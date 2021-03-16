#pragma once
#include "../../dependencies/utilities/utilities.hpp"

class collideable_t {
public:
	vec3_t& mins() {
		using original_fn = vec3_t & (__thiscall*)(void*);
		return (*(original_fn**)this)[1](this);
	}
	vec3_t& maxs() {
		using original_fn = vec3_t & (__thiscall*)(void*);
		return (*(original_fn**)this)[2](this);
	}
	void set_collision_bounds(const vec3_t& mins, const vec3_t& maxs) {
		using original_fn = void(__thiscall*)(void*, const vec3_t&, const vec3_t&);
		static original_fn set_collision_bounds_fn = (original_fn)((DWORD)utilities::pattern_scan("client.dll", "53 8B DC 83 EC 08 83 E4 F8 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 83 EC 10 56 57 8B 7B"));
		set_collision_bounds_fn(this, mins, maxs);
	}
};