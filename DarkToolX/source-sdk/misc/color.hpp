#pragma once
#include <cstdint>
#include <d3d9.h>

struct color {
	int a, r, g, b;
	constexpr color(const int r, const int g, const int b, const int a = 255) noexcept : r(r), g(g), b(b), a(a) { }

	explicit color(const uint32_t color) {
		this->a = (color >> 24) & 0xff;
		this->r = (color >> 16) & 0xff;
		this->g = (color >> 8) & 0xff;
		this->b = (color & 0xff);
	}

	static color from_uint(const uint32_t uint) {
		return color(uint);
	}

	static D3DCOLOR from_color(const color col) {
		return D3DCOLOR_ARGB(col.a, col.r, col.g, col.b);
	}

	static constexpr color black(const int a = 255) { return { 0, 0, 0, a }; }
	static constexpr color white(const int a = 255) { return { 255, 255, 255, a }; }
	static constexpr color red(const int   a = 255) { return { 255, 0, 0, a }; }
	static constexpr color green(const int a = 255) { return { 0, 255, 0, a }; }
	static constexpr color blue(const int  a = 255) { return { 0, 0, 255, a }; }
};
