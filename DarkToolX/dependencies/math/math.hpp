#pragma once
#include "../../source-sdk/sdk.hpp"

namespace math {
	constexpr float M_PI = 3.14159265358979323846f;
	constexpr float M_RADPI = 180 / M_PI;
	constexpr float RAD2DEG(const float x)
	{
		return x * 180.f / M_PI;
	}
	constexpr float DEG2RAD(const float x)
	{
		return x * M_PI / 180.f;
	}
	void correct_movement(const vec3_t& old_angles, c_usercmd* cmd, const float old_forwardmove, const float old_sidemove);
	vec3_t calculate_angle(const vec3_t& a, const vec3_t& b);
	vec3_t angle_vector(const vec3_t& angle);
	vec3_t transform_vector(const vec3_t& a, const matrix_t& b);
	vec3_t vector_angles(const vec3_t& forward);
	vec3_t vector_angles(const vec3_t& forward, const vec3_t& up);
	void angle_vectors(const vec3_t& angles, vec3_t* forward, vec3_t* right, vec3_t* up);
	bool screen_transform(const vec3_t& point, vec3_t& screen);
	bool world_to_screen(const vec3_t& origin, vec2_t& screen);
	vec3_t vector_rotate(const vec3_t& in1, const matrix_t& in2);
	bool IntersectLineWithBB(const vec3_t& vStart, const vec3_t& vEndDelta, const vec3_t& vMin, const vec3_t& vMax);
	bool IntersectLineWithOBB(const vec3_t& vStart, const vec3_t& vEndDelta, const vec3_t& vMin, const vec3_t& vMax, const matrix_t& matrix);
	float fov(const vec3_t& a, const vec3_t& b);
};
