#pragma once
#include "../../source-sdk/math/vector3d.hpp"

class glow_object_definition_t {
public:
	void set(float r, float g, float b, float a) {
		color = vec3_t(r, g, b);
		alpha = a;
		render_when_occluded = true;
		render_when_unoccluded = false;
	}
	bool unused() {
		return next_free_slot != -2;
	}

	int next_free_slot;
	player_t* entity;
	vec3_t color;
	float alpha;
	bool m_bGlowAlphaCappedByRenderAlpha;
	float m_flGlowAlphaFunctionOfMaxVelocity;
	float m_flGlowAlphaMax;
	float m_flGlowPulseOverdrive;
	bool render_when_occluded;
	bool render_when_unoccluded;
	bool full_bloom_render;
	int m_nFullBloomStencilTestValue;
	int render_style;
	int m_nSplitScreenSlot;
};

class glow_manager_t {
public:
	glow_object_definition_t* objects;
	char pad[8];
	int size;
};