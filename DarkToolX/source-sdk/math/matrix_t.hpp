#pragma once
#include "vector3d.hpp"

constexpr float bits_to_float(const std::uint32_t i) {
	union convertor_t {
		float f; unsigned long ul;
	} tmp;

	tmp.ul = i;
	return tmp.f;
}

constexpr std::uint32_t FLOAT32_NAN_BITS = 0x7FC00000;
const float FLOAT32_NAN = bits_to_float(FLOAT32_NAN_BITS);
#define VEC_T_NAN FLOAT32_NAN
#define ASSERT( _exp ) ( (void ) 0 )

struct matrix_t {
	float mat_val[3][4];
	constexpr matrix_t(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23) noexcept : mat_val{ {m00,m01,m02,m03}, {m10,m11,m12,m13}, {m20,m21,m22,m23} }
	{
		/*mat_val[0][0] = m00;	mat_val[0][1] = m01; mat_val[0][2] = m02; mat_val[0][3] = m03;
		mat_val[1][0] = m10;	mat_val[1][1] = m11; mat_val[1][2] = m12; mat_val[1][3] = m13;
		mat_val[2][0] = m20;	mat_val[2][1] = m21; mat_val[2][2] = m22; mat_val[2][3] = m23;*/
	}
	constexpr matrix_t() noexcept : matrix_t(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) {}
	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	constexpr matrix_t(const vec3_t& x, const vec3_t& y, const vec3_t& z, const vec3_t& origin) noexcept : 
		matrix_t(x.x, y.x, z.x, origin.x,
				x.y, y.y, z.y, origin.y,
				x.z, y.z, z.z, origin.z) {}
	constexpr void set_origin(const vec3_t& p) noexcept
	{
		mat_val[0][3] = p.x;
		mat_val[1][3] = p.y;
		mat_val[2][3] = p.z;
	}
	constexpr void invalidate() noexcept
	{
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 4; j++) {
				mat_val[i][j] = VEC_T_NAN;
			}
		}
	}
	constexpr float* operator[](int i) noexcept
	{
		ASSERT((i >= 0) && (i < 3));
		return mat_val[i];
	}
	constexpr const float* operator[](int i) const noexcept
	{
		ASSERT((i >= 0) && (i < 3));
		return mat_val[i];
	}
	constexpr float* base() noexcept
	{
		return &mat_val[0][0];
	}
	constexpr const float* base() const noexcept
	{
		return &mat_val[0][0];
	}
};
