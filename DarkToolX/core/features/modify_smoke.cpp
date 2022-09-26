#include "features.hpp"

constexpr std::array smoke_materials
{
	"particle/vistasmokev1/vistasmokev1_emods",
	"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	"particle/vistasmokev1/vistasmokev1_fire",
	"particle/vistasmokev1/vistasmokev1_smokegrenade"
};

void features::modify_smoke()
{
	if (!csgo::cfg.view().modify_smoke)
		return;

	for (const auto mat : smoke_materials) {
		const auto material = interfaces::material_system->find_material(mat, TEXTURE_GROUP_OTHER);
		const auto flag = csgo::cfg.view().modify_smoke == 1 ? material_var_wireframe : material_var_no_draw;

		material->set_material_var_flag(flag, true);
	}
}