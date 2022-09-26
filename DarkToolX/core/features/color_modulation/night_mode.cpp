#include "../features.hpp"

void features::color_modulation::night_mode::static_prop(float& r, float& g, float& b)
{
	if (!csgo::cfg.view().night_mode)
		return;

	r *= 0.45f;
	g *= 0.45f;
	b *= 0.45f;
}

void features::color_modulation::night_mode::sky_box(float& r, float& g, float& b)
{
	if (!csgo::cfg.view().dark_skybox)
		return;

	r *= 0.15f;
	g *= 0.15f;
	b *= 0.15f;
}

void features::color_modulation::night_mode::world(float& r, float& g, float& b)
{
	if (!csgo::cfg.view().night_mode)
		return;

	r *= 0.15f;
	g *= 0.15f;
	b *= 0.15f;
}