#include "../features.hpp"

static void clear_ref_counted_vector(utl_vector<i_ref_counted*>& vec) {
	for (auto& e : vec) {
		if (e) {
			e->unreference();
			e = nullptr;
		}
	}
	vec.remove_all();
}

bool features::util::can_force_item_update(weapon_t* item)
{
	for (auto& e : item->m_CustomMaterials()) {
		if (e) {
			const auto& is_valid = *(bool*)(e + 4 + 20+4);
			if (!is_valid)
				return false;
		}
	}
	return true;
}

void features::util::force_item_update(weapon_t* item) {
	auto& view = item->m_Item();
	item->m_bCustomMaterialInitialized() = (item->paint_kit() <= 0);
	item->m_CustomMaterials().remove_all();
	view.m_CustomMaterials().remove_all();
	clear_ref_counted_vector(view.m_VisualsDataProcessors());
	item->net_post_data_update(0);
	item->net_on_data_changed(0);
}