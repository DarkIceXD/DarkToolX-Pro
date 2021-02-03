#include "interfaces.hpp"
#include "../utilities/csgo.hpp"

bool interfaces::initialize() {
	client = get_interface<i_base_client_dll, interface_type::index>("client.dll", "VClient018");
	entity_list = get_interface<i_client_entity_list, interface_type::index>("client.dll", "VClientEntityList003");
	engine = get_interface<iv_engine_client, interface_type::index>("engine.dll", "VEngineClient014");
	panel = get_interface<i_panel, interface_type::index>("vgui2.dll", "VGUI_Panel009");
	surface = get_interface<i_surface, interface_type::index>("vguimatsurface.dll", "VGUI_Surface031");
	material_system = get_interface<i_material_system, interface_type::index>("materialsystem.dll", "VMaterialSystem080");
	model_info = get_interface<iv_model_info, interface_type::index>("engine.dll", "VModelInfoClient004");
	model_render = get_interface<iv_model_render, interface_type::index>("engine.dll", "VEngineModel016");
	render_view = get_interface<i_render_view, interface_type::index>("engine.dll", "VEngineRenderView014");
	console = get_interface<i_console, interface_type::index>("vstdlib.dll", "VEngineCvar007");
	localize = get_interface<i_localize, interface_type::index>("localize.dll", "Localize_001");
	event_manager = get_interface<i_game_event_manager2, interface_type::index>("engine.dll", "GAMEEVENTSMANAGER002");
	debug_overlay = get_interface<iv_debug_overlay, interface_type::index>("engine.dll", "VDebugOverlay004");
	inputsystem = get_interface<i_inputsytem, interface_type::index>("inputsystem.dll", "InputSystemVersion001");
	trace_ray = get_interface<trace, interface_type::index>("engine.dll", "EngineTraceClient004");
	game_movement = get_interface<player_game_movement, interface_type::index>("client.dll", "GameMovement001");
	prediction = get_interface<player_prediction, interface_type::index>("client.dll", "VClientPrediction001");

	physics_surface_props = get_interface<i_physics_surface_props, interface_type::index>("vphysics.dll", "VPhysicsSurfaceProps001");
	game_types = get_interface<i_game_types, interface_type::index>("matchmaking.dll", "VENGINE_GAMETYPES_VERSION002");
	engine_sound = get_interface<i_engine_sound, interface_type::index>("engine.dll", "IEngineSoundClient003");
	render_beams = *reinterpret_cast<i_view_render_beams**>(utilities::pattern_scan("client.dll", "A1 ? ? ? ? 56 8B F1 B9 ? ? ? ? FF 50 08") + 1);
	/*custom interfaces*/
	clientmode = **reinterpret_cast<i_client_mode * **>((*reinterpret_cast<uintptr_t * *>(client))[10] + 5);
	globals = **reinterpret_cast<c_global_vars_base***>((*reinterpret_cast<uintptr_t**>(client))[11] + 10);

	clientstate = **reinterpret_cast<i_client_state***>(utilities::pattern_scan("engine.dll", "A1 ? ? ? ? 8B 80 ? ? ? ? C3") + 1);
	directx = **reinterpret_cast<IDirect3DDevice9***>(utilities::pattern_scan("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);
	input = *reinterpret_cast<i_input**>(utilities::pattern_scan("client.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 1);
	glow_manager = reinterpret_cast<glow_manager_t*>(*reinterpret_cast<uintptr_t*>(utilities::pattern_scan("client.dll", "0F 11 05 ? ? ? ? 83 C8 01 C7 05 ? ? ? ? 00 00 00 00") + 3));
	move_helper = **reinterpret_cast<player_move_helper***>(utilities::pattern_scan("client.dll", "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01") + 2);
	weapon_system = *reinterpret_cast<i_weapon_system**>(utilities::pattern_scan("client.dll", "8B 35 ? ? ? ? FF 10 0F B7 C0") + 2);
	console::log("[setup] interfaces initialized!\n");
	return true;
}
