#include "features.hpp"

static int random_sequence(const int low, const int high)
{
	return rand() % (high - low + 1) + low;
}

static int get_new_animation(const short index, const int sequence)
{
	enum ESequence
	{
		SEQUENCE_DEFAULT_DRAW = 0,
		SEQUENCE_DEFAULT_IDLE1 = 1,
		SEQUENCE_DEFAULT_IDLE2 = 2,
		SEQUENCE_DEFAULT_LIGHT_MISS1 = 3,
		SEQUENCE_DEFAULT_LIGHT_MISS2 = 4,
		SEQUENCE_DEFAULT_HEAVY_MISS1 = 9,
		SEQUENCE_DEFAULT_HEAVY_HIT1 = 10,
		SEQUENCE_DEFAULT_HEAVY_BACKSTAB = 11,
		SEQUENCE_DEFAULT_LOOKAT01 = 12,

		SEQUENCE_BUTTERFLY_DRAW = 0,
		SEQUENCE_BUTTERFLY_DRAW2 = 1,
		SEQUENCE_BUTTERFLY_LOOKAT01 = 13,
		SEQUENCE_BUTTERFLY_LOOKAT03 = 15,

		SEQUENCE_FALCHION_IDLE1 = 1,
		SEQUENCE_FALCHION_HEAVY_MISS1 = 8,
		SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP = 9,
		SEQUENCE_FALCHION_LOOKAT01 = 12,
		SEQUENCE_FALCHION_LOOKAT02 = 13,

		SEQUENCE_CSS_LOOKAT01 = 14,
		SEQUENCE_CSS_LOOKAT02 = 15,

		SEQUENCE_DAGGERS_IDLE1 = 1,
		SEQUENCE_DAGGERS_LIGHT_MISS1 = 2,
		SEQUENCE_DAGGERS_LIGHT_MISS5 = 6,
		SEQUENCE_DAGGERS_HEAVY_MISS2 = 11,
		SEQUENCE_DAGGERS_HEAVY_MISS1 = 12,

		SEQUENCE_BOWIE_IDLE1 = 1,
	};

	switch (index)
	{
	case WEAPON_KNIFE_BUTTERFLY:
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_DRAW:
			return random_sequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random_sequence(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
		default:
			return sequence + 1;
		}
	}
	case WEAPON_KNIFE_FALCHION:
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_FALCHION_IDLE1;
		case SEQUENCE_DEFAULT_HEAVY_MISS1:
			return random_sequence(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random_sequence(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		default:
			return sequence - 1;
		}
	}
	case WEAPON_KNIFE_CSS:
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random_sequence(SEQUENCE_CSS_LOOKAT01, SEQUENCE_CSS_LOOKAT02);
		default:
			return sequence;
		}
	}
	case WEAPON_KNIFE_PUSH:
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_DAGGERS_IDLE1;
		case SEQUENCE_DEFAULT_LIGHT_MISS1:
		case SEQUENCE_DEFAULT_LIGHT_MISS2:
			return random_sequence(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
		case SEQUENCE_DEFAULT_HEAVY_MISS1:
			return random_sequence(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
		case SEQUENCE_DEFAULT_HEAVY_HIT1:
		case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
		case SEQUENCE_DEFAULT_LOOKAT01:
			return sequence + 3;
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		default:
			return sequence + 2;
		}
	}
	case WEAPON_KNIFE_SURVIVAL_BOWIE:
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_BOWIE_IDLE1;
		default:
			return sequence - 1;
		}
	}
	case WEAPON_KNIFE_URSUS:
	case WEAPON_KNIFE_CORD:
	case WEAPON_KNIFE_CANIS:
	case WEAPON_KNIFE_OUTDOOR:
	case WEAPON_KNIFE_SKELETON:
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_DRAW:
			return random_sequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random_sequence(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
		default:
			return sequence + 1;
		}
	}
	case WEAPON_KNIFE_STILETTO:
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random_sequence(12, 13);
		default:
			return sequence;
		}
	}
	case WEAPON_KNIFE_WIDOWMAKER:
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random_sequence(14, 15);
		default:
			return sequence;
		}
	}
	default:
		return sequence;
	}
}

void features::sequence_remapping(c_recv_proxy_data* data, base_view_model_t* entity)
{
	if (!csgo::local_player)
		return;

	if (!csgo::local_player->is_alive())
		return;

	const auto owner = (player_t*)interfaces::entity_list->get_client_entity_handle(entity->owner());
	if (owner != csgo::local_player)
		return;

	const auto view_model_weapon = (weapon_t*)interfaces::entity_list->get_client_entity_handle(entity->weapon());
	if (!view_model_weapon)
		return;

	auto& sequence = data->value.m_int;
	sequence = get_new_animation(view_model_weapon->item_definition_index(), sequence);
}