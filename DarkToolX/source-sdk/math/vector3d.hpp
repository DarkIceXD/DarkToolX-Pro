#pragma once
#include <limits>
#include <algorithm>
#include <cmath>

class vec3_t {
public:
	float x, y, z;
	constexpr vec3_t(const float x, const float y, const float z) noexcept : x(x), y(y), z(z) {}
	constexpr vec3_t() noexcept : vec3_t(0, 0, 0) {}
	constexpr void init(float _x, float _y, float _z) noexcept {
		x = _x; y = _y; z = _z;
	}
	constexpr float& operator[](int i) noexcept {
		return ((float*)this)[i];
	}
	constexpr float operator[](int i) const noexcept {
		return ((float*)this)[i];
	}
	constexpr float length_sqr() const noexcept
	{
		return x * x + y * y + z * z;
	}
	float length() const noexcept
	{
		return sqrt(length_sqr());
	}
	constexpr vec3_t& operator+=(const vec3_t& obj) noexcept
	{
		this->x += obj.x;
		this->y += obj.y;
		this->z += obj.z;
		return *this;
	}
	constexpr vec3_t& operator-=(const vec3_t& obj) noexcept
	{
		this->x -= obj.x;
		this->y -= obj.y;
		this->z -= obj.z;
		return *this;
	}
	constexpr vec3_t& operator*=(const float c) noexcept
	{
		this->x *= c;
		this->y *= c;
		this->z *= c;
		return *this;
	}
	constexpr vec3_t& operator/=(const float c) noexcept
	{
		this->x /= c;
		this->y /= c;
		this->z /= c;
		return *this;
	}
	constexpr friend vec3_t operator*(vec3_t lhs, const float c) noexcept
	{
		lhs *= c;
		return lhs;
	}
	constexpr friend vec3_t operator/(vec3_t lhs, const float c) noexcept
	{
		lhs /= c;
		return lhs;
	}
	constexpr friend vec3_t operator+(vec3_t lhs, const vec3_t& rhs) noexcept
	{
		lhs += rhs;
		return lhs;
	}
	constexpr friend vec3_t operator-(vec3_t lhs, const vec3_t& rhs) noexcept
	{
		lhs -= rhs;
		return lhs;
	}
	float length_2d() const noexcept
	{
		return sqrt((x * x) + (y * y));
	}
	void clamp() noexcept
	{
		x = std::clamp(x, -89.0f, 89.0f);
		y = std::clamp(std::remainder(y, 360.0f), -180.0f, 180.0f);
		z = std::clamp(z, -50.0f, 50.0f);
	}
	vec3_t clamped() const noexcept
	{
		vec3_t clamped = *this;
		clamped.clamp();
		return clamped;
	}
	void normalize_angles() noexcept
	{
		x = std::isfinite(x) ? std::remainder(x, 360.0f) : 0.0f;
		y = std::isfinite(y) ? std::remainder(y, 360.0f) : 0.0f;
		z = 0.0f;
	}
	vec3_t normalized_angles() const noexcept
	{
		vec3_t vec(*this);
		vec.normalize_angles();
		return vec;
	}
	float normalize() noexcept
	{
		const auto len = length();
		if (len != 0)
			*this /= len;
		return len;
	}
	vec3_t normalized() const noexcept
	{
		vec3_t vec(*this);
		vec.normalize();
		return vec;
	}
	static constexpr vec3_t crossproduct(const vec3_t v1, const vec3_t v2) noexcept
	{
		return { (v1.y * v2.z) - (v1.z * v2.y), -((v1.x * v2.z) - (v1.z * v2.x)), (v1.x * v2.y) - (v1.y * v2.x) };
	}
	constexpr vec3_t cross(const vec3_t& other) const noexcept
	{
		return crossproduct(*this, other);
	}
	constexpr float dot(const vec3_t other) const noexcept
	{
		return (x * other.x + y * other.y + z * other.z);
	}
	constexpr float dot(const float* other) const noexcept
	{
		return (x * other[0] + y * other[1] + z * other[2]);
	}
};