#include "features.hpp"

class radar_player_t
{
public:
	vec3_t pos; //0x0000 
	vec3_t angle; //0x000C 
	vec3_t spotted_map_angle_related; //0x0018 
	DWORD tab_related; //0x0024 
	char pad_0x0028[0xC]; //0x0028
	float spotted_time; //0x0034 
	float spotted_fraction; //0x0038 
	float time; //0x003C 
	char pad_0x0040[0x4]; //0x0040
	__int32 player_index; //0x0044 
	__int32 entity_index; //0x0048 
	char pad_0x004C[0x4]; //0x004C
	__int32 health; //0x0050 
	char name[32]; //0x785888 
	char pad_0x0074[0x75]; //0x0074
	unsigned char spotted; //0x00E9 
	char pad_0x00EA[0x8A]; //0x00EA
}; //Size=0x0174

class CCSGO_HudRadar
{
public:
	char pad_0x0000[0x14C];
	radar_player_t radar_info[65];
};

void features::dormant()
{
	if (!csgo::conf->visuals().dormant_esp)
		return;

	static auto FindHudElement = (uintptr_t(__thiscall*)(void*, const char*))utilities::pattern_scan("client.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28");
	static auto hud_ptr = *(uintptr_t**)(utilities::pattern_scan("client.dll", "81 25 ? ? ? ? ? ? ? ? 8B 01") + 2);

	auto radar_base = FindHudElement(hud_ptr, "CCSGO_HudRadar");
	CCSGO_HudRadar* hud_radar = (CCSGO_HudRadar*)(radar_base - 20);
	if (!hud_radar)
		return;

	for (auto i = 1; i <= interfaces::globals->max_clients; i++)
	{
		auto entity = static_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity || !entity->dormant() || !entity->is_player() || entity == csgo::local_player)
			continue;

		if (!entity->is_alive())
			continue;

		if (!entity->spotted())
			continue;

		const auto& radar_entity = hud_radar->radar_info[i];
		entity->set_position(radar_entity.pos);
	}
}