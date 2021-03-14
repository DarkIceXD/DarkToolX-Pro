#include "features.hpp"

static bool traceToExit(const trace_t& enterTrace, const vec3_t& start, const vec3_t& direction, vec3_t& end, trace_t& exitTrace)
{
	static uintptr_t traceToExitFn = (uintptr_t)utilities::pattern_scan("client.dll", "55 8B EC 83 EC 30 F3 0F 10 75");
	bool result;
	__asm {
		push exitTrace
		mov eax, direction
		push[eax]vec3_t.z
		push[eax]vec3_t.y
		push[eax]vec3_t.x
		mov eax, start
		push[eax]vec3_t.z
		push[eax]vec3_t.y
		push[eax]vec3_t.x
		mov edx, enterTrace
		mov ecx, end
		call traceToExitFn
		add esp, 28
		mov result, al
	}
	return result;
}

constexpr bool is_armored(const int hitGroup, const bool helmet)
{
	switch (hitGroup) {
	case hitgroup_head:
		return helmet;
	case hitgroup_chest:
	case hitgroup_stomach:
	case hitgroup_leftarm:
	case hitgroup_rightarm:
		return true;
	default:
		return false;
	}
}

static void scale_damage(player_t* entity, const int hit_group, const weapon_info_t* weaponData, float& currentDamage)
{
	const auto hasHeavyArmor = entity->has_heavy_armor();

	switch (hit_group)
	{
	case hitgroups::hitgroup_head:
		currentDamage *= hasHeavyArmor ? 2.f : 4.f; //Heavy Armor does 1/2 damage
		break;
	case hitgroups::hitgroup_stomach:
		currentDamage *= 1.25f;
		break;
	case hitgroups::hitgroup_leftleg:
	case hitgroups::hitgroup_rightleg:
		currentDamage *= 0.75f;
		break;
	default:
		break;
	}

	const auto armorValue = entity->armor();
	if (armorValue > 0 && is_armored(hit_group, entity->has_helmet()))
	{
		float bonusValue = 1.f, armorBonusRatio = 0.5f, armorRatio = weaponData->weapon_armor_ratio / 2.f;
		if (hasHeavyArmor)
		{
			armorBonusRatio = 0.33f;
			armorRatio *= 0.5f;
			bonusValue = 0.33f;
		}

		auto NewDamage = currentDamage * armorRatio;

		if (hasHeavyArmor)
			NewDamage *= 0.85f;

		if (((currentDamage - currentDamage * armorRatio) * (bonusValue * armorBonusRatio)) > armorValue)
			NewDamage = currentDamage - armorValue / armorBonusRatio;

		currentDamage = NewDamage;
	}
}

static float handle_bulletpenetration(const trace_t& enterTrace, const vec3_t& direction, const float penetration, const float damage, vec3_t& result)
{
	const auto enterSurfaceData = interfaces::physics_surface_props->get_surface_data(enterTrace.surface.surfaceProps);
	if (enterSurfaceData->penetrationModifier < 0.1f)
		return 0;
	vec3_t end;
	trace_t exitTrace;
	if (!traceToExit(enterTrace, enterTrace.end, direction, end, exitTrace))
		return 0;

	auto exitSurfaceData = interfaces::physics_surface_props->get_surface_data(exitTrace.surface.surfaceProps);
	auto damageModifier = 0.16f;
	auto penetrationModifier = (enterSurfaceData->penetrationModifier + exitSurfaceData->penetrationModifier) / 2.0f;
	if (enterSurfaceData->material == CHAR_TEX_GRATE || enterSurfaceData->material == CHAR_TEX_GLASS)
	{
		damageModifier = 0.05f;
		penetrationModifier = 3.0f;
	}
	else if (enterTrace.contents >> 3 & CONTENTS_SOLID || enterTrace.surface.flags >> 7 & SURF_LIGHT)
	{
		penetrationModifier = 1.0f;
	}

	if (enterSurfaceData->material == exitSurfaceData->material) {
		if (exitSurfaceData->material == CHAR_TEX_CARDBOARD || exitSurfaceData->material == CHAR_TEX_WOOD)
			penetrationModifier = 3.0f;
		else if (exitSurfaceData->material == CHAR_TEX_PLASTIC)
			penetrationModifier = 2.0f;
	}
	result = exitTrace.end;
	return damage - ((11.25f / penetration + (exitTrace.end - enterTrace.end).length_sqr() / 24.0f) / penetrationModifier + damage * damageModifier);
}

auto_wall_data features::auto_wall(vec3_t direction, const weapon_info_t* weapon_data, const bool bangable)
{
	auto_wall_data data{ nullptr, static_cast<float>(weapon_data->weapon_damage) };
	auto start(csgo::local_player->get_eye_pos());
	direction.normalize();
	auto max_range = weapon_data->weapon_range;
	trace_filter filter(csgo::local_player);
	auto current_distance = 0.f;
	while (data.damage >= 1.0f) {
		max_range -= current_distance;
		trace_t trace;
		ray_t ray(start, start + direction * max_range);
		interfaces::trace_ray->trace_ray(ray, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &trace);
		if (!trace.did_hit())
			break;
		current_distance += trace.flFraction * max_range;
		data.damage *= pow(weapon_data->weapon_range_mod, current_distance / 500.f);
		if (trace.entity && csgo::local_player->is_enemy(trace.entity) && hitgroup_generic < trace.hitGroup && trace.hitGroup < hitgroup_gear) {
			scale_damage(trace.entity, trace.hitGroup, weapon_data, data.damage);
			data.entity = trace.entity;
			return data;
		}
		if (!bangable)
			break;

		data.damage = handle_bulletpenetration(trace, direction, weapon_data->weapon_penetration, data.damage, start);
	}
	return { nullptr, 0 };
}