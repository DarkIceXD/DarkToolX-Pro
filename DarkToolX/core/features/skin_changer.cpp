#include "features.hpp"

static create_client_class_fn get_wearable_create_fn()
{
	auto clazz = interfaces::client->get_client_classes();
	while (clazz->class_id != ceconwearable)
		clazz = clazz->next_ptr;
	return clazz->create_fn;
}

static weapon_t* make_glove(int entry, int serial)
{
	static auto create_wearable_fn = get_wearable_create_fn();
	create_wearable_fn(entry, serial);
	const auto glove = static_cast<weapon_t*>(interfaces::entity_list->get_client_entity(entry));
	/*static const auto set_abs_origin_fn = reinterpret_cast<void(__thiscall*)(void*, const vec3_t&)>(utilities::pattern_scan("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1"));
	set_abs_origin_fn(glove, { 10000.f, 10000.f, 10000.f });*/
	return glove;
}

constexpr const char* get_model_name(const short item_index)
{
	switch (item_index)
	{
	case WEAPON_KNIFE_BAYONET: return "models/weapons/v_knife_bayonet.mdl";
	case WEAPON_KNIFE_FLIP: return "models/weapons/v_knife_flip.mdl";
	case WEAPON_KNIFE_GUT: return "models/weapons/v_knife_gut.mdl";
	case WEAPON_KNIFE_KARAMBIT: return "models/weapons/v_knife_karam.mdl";
	case WEAPON_KNIFE_M9_BAYONET: return "models/weapons/v_knife_m9_bay.mdl";
	case WEAPON_KNIFE_TACTICAL: return "models/weapons/v_knife_tactical.mdl";
	case WEAPON_KNIFE_FALCHION: return "models/weapons/v_knife_falchion_advanced.mdl";
	case WEAPON_KNIFE_SURVIVAL_BOWIE: return "models/weapons/v_knife_survival_bowie.mdl";
	case WEAPON_KNIFE_BUTTERFLY: return "models/weapons/v_knife_butterfly.mdl";
	case WEAPON_KNIFE_PUSH: return "models/weapons/v_knife_push.mdl";
	case WEAPON_KNIFE_URSUS: return "models/weapons/v_knife_ursus.mdl";
	case WEAPON_KNIFE_GYPSY_JACKKNIFE: return "models/weapons/v_knife_gypsy_jackknife.mdl";
	case WEAPON_KNIFE_STILETTO: return "models/weapons/v_knife_stiletto.mdl";
	case WEAPON_KNIFE_WIDOWMAKER: return "models/weapons/v_knife_widowmaker.mdl";
	case WEAPON_KNIFE_CSS: return "models/weapons/v_knife_css.mdl";
	case WEAPON_KNIFE_CORD: return "models/weapons/v_knife_cord.mdl";
	case WEAPON_KNIFE_CANIS: return "models/weapons/v_knife_canis.mdl";
	case WEAPON_KNIFE_OUTDOOR: return "models/weapons/v_knife_outdoor.mdl";
	case WEAPON_KNIFE_SKELETON: return "models/weapons/v_knife_skeleton.mdl";
	case GLOVE_BROKEN_FANG: return "models/weapons/w_models/arms/w_glove_bloodhound_brokenfang.mdl";
	case GLOVE_STUDDED_BLOODHOUND: return "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl";
	case GLOVE_SPORTY: return "models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl";
	case GLOVE_SLICK: return "models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl";
	case GLOVE_LEATHER_WRAP: return "models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl";
	case GLOVE_MOTORCYCLE: return "models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl";
	case GLOVE_SPECIALIST: return "models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl";
	case GLOVE_HYDRA: return "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl";
	default: return nullptr;
	}
}

static void change_model(weapon_t& item, const int new_index)
{
	const auto model_name = get_model_name(new_index);
	if (model_name)
	{
		item.item_definition_index() = new_index;
		item.set_model_index(interfaces::model_info->get_model_index(model_name));
		item.net_pre_data_update(0);
	}
}

static void apply_skin(weapon_t& item, const config::skin& skin)
{
	if (skin.paint_kit)
	{
		item.item_id_high() = -1;
		item.account_id() = item.original_owner_xuid();
		item.paint_kit() = skin.paint_kit;
		if (skin.quality)
			item.entity_quality() = skin.quality;
		if (!skin.nametag.empty())
			strcpy_s(item.custom_name(), skin.nametag.c_str());
		if (skin.seed)
			item.seed() = skin.seed;
		if (skin.stat_trak)
			item.stat_trak() = skin.stat_trak;
		item.wear() = skin.wear;
	}
}

static void apply_skin(weapon_t& item, const config::skin_animation_frame& skin)
{
	if (item.item_id_high() == -1)
	{
		item.account_id() = item.original_owner_xuid();
		if (skin.paint_kit)
			item.paint_kit() = skin.paint_kit;
		if (!skin.nametag.empty())
			strcpy_s(item.custom_name(), skin.nametag.c_str());
		if (skin.seed)
			item.seed() = skin.seed;
		if (skin.stat_trak)
			item.stat_trak() = skin.stat_trak;
		item.wear() = skin.wear;
	}
}

void features::skin_changer() {
	if (!csgo::conf->skin_changer().enabled)
		return;

	if (!csgo::local_player)
		return;

	static auto ticks_to_skip = 0;
	if (ticks_to_skip > 0)
	{
		ticks_to_skip--;
	}
	else
	{
		const auto weapon = csgo::local_player->active_weapon();
		if (weapon)
		{
			const auto found = csgo::conf->skin_changer().get_skin(weapon->item_definition_index());
			if (found)
			{
				const auto animation = found->next_animation();
				if (animation && features::can_force_item_update(weapon))
				{
					apply_skin(*weapon, *animation);
					force_item_update(weapon);
					ticks_to_skip = found->delay_ticks;
				}
			}
		}
	}


	{ //gloves
		const auto wearables = csgo::local_player->get_wearables();
		static unsigned int glove_handle = 0;
		auto glove = static_cast<weapon_t*>(interfaces::entity_list->get_client_entity_handle(wearables[0]));
		if (!glove) // There is no glove
		{
			// Try to get our last created glove
			const auto our_glove = static_cast<weapon_t*>(interfaces::entity_list->get_client_entity_handle(glove_handle));
			if (our_glove) // Our glove still exists
			{
				wearables[0] = glove_handle;
				glove = our_glove;
			}
		}
		if (!csgo::local_player->is_alive())
		{
			// We are dead but we have a glove, destroy it
			if (glove)
			{
				glove->net_set_destroyed_on_recreate_entities();
				glove->net_release();
			}
			return;
		}
		// We don't have a glove, but we should
		if (!glove)
		{
			const auto entry = interfaces::entity_list->get_highest_index() + 1;
			const auto serial = rand() % 0x1000;
			glove = make_glove(entry, serial);
			wearables[0] = entry | serial << 16;
			// Let's store it in case we somehow lose it.
			glove_handle = wearables[0];
		}
		glove->get_index() = -1;
		apply_skin(*glove, csgo::conf->skin_changer().skins.at(0));
		change_model(*glove, csgo::conf->skin_changer().get_glove_index());
	}
	auto weapons = csgo::local_player->get_weapons();
	for (auto i = 0; i < 64; i++)
	{
		auto weapon = static_cast<weapon_t*>(interfaces::entity_list->get_client_entity_handle(weapons[i]));
		if (!weapon)
			break;

		if (weapon->item_id_high() == -1)
			continue;

		auto& index = weapon->item_definition_index();
		const auto found = csgo::conf->skin_changer().get_skin(index);
		if (index == WEAPON_KNIFE_T || index == WEAPON_KNIFE)
			change_model(*weapon, csgo::conf->skin_changer().get_knife_index());
		if (found)
			apply_skin(*weapon, *found);
	}
	const auto view_model = static_cast<base_view_model_t*>(interfaces::entity_list->get_client_entity_handle(csgo::local_player->view_model()));
	if (!view_model)
		return;

	const auto view_model_weapon = static_cast<weapon_t*>(interfaces::entity_list->get_client_entity_handle(view_model->weapon()));
	if (!view_model_weapon)
		return;

	const auto model_name = get_model_name(view_model_weapon->item_definition_index());
	if (!model_name)
		return;

	const auto override_model_index = interfaces::model_info->get_model_index(model_name);
	view_model->model_index() = override_model_index;
}