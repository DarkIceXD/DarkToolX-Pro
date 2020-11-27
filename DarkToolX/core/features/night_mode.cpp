#include "features.hpp"

void features::night_mode(i_material* mat, float& r, float& g, float& b)
{
	if (!csgo::conf->view().night_mode)
		return;

	if (!mat || mat->is_error_material())
		return;

	switch(fnv::hash(mat->get_texture_group_name()))
	{
	case fnv::hash("StaticProp textures"):
		r *= 0.45f;
		g *= 0.45f;
		b *= 0.45f;
		break;
	case fnv::hash("SkyBox textures"):
		if (!csgo::conf->view().dark_skybox)
			break;
	case fnv::hash("World textures"):
		r *= 0.15f;
		g *= 0.15f;
		b *= 0.15f;
		break;
	}
}