#include "../../dependencies/minhook/minhook.h"
#include "../../dependencies/utilities/csgo.hpp"
#include "../features/features.hpp"
#include "../features/engine_prediction.hpp"
#include "event_listener.h"
#include "../../dependencies/utilities/netvars/recv_proxy_hook.hpp"
#include "../menu/menu.h"
#include <intrin.h>

hooks::create_move::fn create_move_original = nullptr;
hooks::paint_traverse::fn paint_traverse_original = nullptr;
hooks::override_view::fn override_view_original = nullptr;
hooks::do_post_screen_effects::fn do_post_screen_effects_original = nullptr;
hooks::frame_stage_notify::fn frame_stage_notify_original = nullptr;
hooks::sv_teamid_overhead_get_int::fn sv_teamid_overhead_get_int_original = nullptr;
hooks::emit_sound::fn emit_sound_original = nullptr;
hooks::end_scene::fn end_scene_original = nullptr;
hooks::reset::fn reset_original = nullptr;
hooks::lock_cursor::fn lock_cursor_original = nullptr;
hooks::get_color_modulation::fn get_color_modulation_original = nullptr;
hooks::is_using_static_prop_debug_modes::fn is_using_static_prop_debug_modes_original = nullptr;
hooks::should_draw_fog::fn should_draw_fog_original = nullptr;
hooks::should_skip_animation_frame::fn should_skip_animation_frame_original = nullptr;
hooks::do_procedural_foot_plant::fn do_procedural_foot_plant_original = nullptr;
hooks::build_transformations::fn build_transformations_original = nullptr;
hooks::check_for_sequence_change::fn check_for_sequence_change_original = nullptr;
hooks::is_hltv::fn is_hltv_original = nullptr;
hooks::standard_blending_rules::fn standard_blending_rules_original = nullptr;
hooks::calculate_view::fn calculate_view_original = nullptr;
hooks::modify_eye_position::fn modify_eye_position_original = nullptr;
static event_listener* listener = nullptr;
static recv_prop_hook* sequence_hook = nullptr;
static HWND window = nullptr;
static WNDPROC original_wndproc = nullptr;

extern IMGUI_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool hooks::initialize() {
	const auto create_move_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 24));
	const auto paint_traverse_target = reinterpret_cast<void*>(get_virtual(interfaces::panel, 41));
	const auto override_view_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 18));
	const auto do_post_screen_effects_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 44));
	const auto frame_stage_notify_target = reinterpret_cast<void*>(get_virtual(interfaces::client, 37));
	const auto sv_teamid_overhead_get_int_target = reinterpret_cast<void*>(get_virtual(interfaces::console->get_convar("sv_teamid_overhead"), 13));
	const auto emit_sound_target = reinterpret_cast<void*>(get_virtual(interfaces::engine_sound, 5));
	const auto end_scene_target = reinterpret_cast<void*>(get_virtual(interfaces::directx, 42));
	const auto reset_target = reinterpret_cast<void*>(get_virtual(interfaces::directx, 16));
	const auto lock_cursor_target = reinterpret_cast<void*>(get_virtual(interfaces::surface, 67));
	const auto get_color_modulation_target = static_cast<void*>(utilities::pattern_scan("materialsystem.dll", "55 8B EC 83 EC ? 56 8B F1 8A 46"));
	const auto is_using_static_prop_debug_modes_target = static_cast<void*>(utilities::pattern_scan("engine.dll", "8B 0D ? ? ? ? 81 F9 ? ? ? ? 75 ? A1 ? ? ? ? 35 ? ? ? ? EB ? 8B 01 FF 50 ? 83 F8 ? 0F 85 ? ? ? ? 8B 0D"));
	const auto should_draw_fog_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 17));
	const auto should_skip_animation_frame_target = static_cast<void*>(utilities::pattern_scan("client.dll", "57 8B F9 8B 07 8B 80 ? ? ? ? FF D0 84 C0 75 02"));
	const auto do_procedural_foot_plant_target = static_cast<void*>(utilities::pattern_scan("client.dll", "55 8B EC 83 E4 F0 83 EC 78 56 8B F1 57 8B 56"));
	const auto build_transformations_target = static_cast<void*>(utilities::pattern_scan("client.dll", "55 8B EC 56 8B 75 18 57"));
	const auto check_for_sequence_change_target = static_cast<void*>(utilities::pattern_scan("client.dll", "55 8B EC 51 53 8B 5D 08 56 8B F1 57 85"));
	const auto is_hltv_target = reinterpret_cast<void*>(get_virtual(interfaces::engine, 93));
	const auto standard_blending_rules_target = static_cast<void*>(utilities::pattern_scan("client.dll", "55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? 56 8B 75 08 57 8B F9 85 F6"));
	const auto calculate_view_target = static_cast<void*>(utilities::pattern_scan("client.dll", "55 8B EC 83 EC 14 53 56 57 FF 75 18"));
	const auto modify_eye_position_target = static_cast<void*>(utilities::pattern_scan("client.dll", "55 8B EC 83 E4 F8 83 EC 5C 53 8B D9 56 57 83"));
	sequence_hook = new recv_prop_hook(base_view_model_t::sequence_prop(), &hooks::sequence_proxy::hook);
	if (MH_Initialize() != MH_OK)
		throw std::runtime_error("failed to initialize MH_Initialize");

	if (MH_CreateHook(create_move_target, &create_move::hook, reinterpret_cast<void**>(&create_move_original)) != MH_OK)
		throw std::runtime_error("failed to initialize create_move");

	if (MH_CreateHook(paint_traverse_target, &paint_traverse::hook, reinterpret_cast<void**>(&paint_traverse_original)) != MH_OK)
		throw std::runtime_error("failed to initialize paint_traverse");

	if (MH_CreateHook(override_view_target, &override_view::hook, reinterpret_cast<void**>(&override_view_original)) != MH_OK)
		throw std::runtime_error("failed to initialize override_view");

	if (MH_CreateHook(do_post_screen_effects_target, &do_post_screen_effects::hook, reinterpret_cast<void**>(&do_post_screen_effects_original)) != MH_OK)
		throw std::runtime_error("failed to initialize do_post_screen_effects");

	if (MH_CreateHook(frame_stage_notify_target, &frame_stage_notify::hook, reinterpret_cast<void**>(&frame_stage_notify_original)) != MH_OK)
		throw std::runtime_error("failed to initialize frame_stage_notify");

	if (MH_CreateHook(sv_teamid_overhead_get_int_target, &sv_teamid_overhead_get_int::hook, reinterpret_cast<void**>(&sv_teamid_overhead_get_int_original)) != MH_OK)
		throw std::runtime_error("failed to initialize sv_teamid_overhead_get_int");

	if (MH_CreateHook(emit_sound_target, &emit_sound::hook, reinterpret_cast<void**>(&emit_sound_original)) != MH_OK)
		throw std::runtime_error("failed to initialize emit_sound");

	if (MH_CreateHook(end_scene_target, &end_scene::hook, reinterpret_cast<void**>(&end_scene_original)) != MH_OK)
		throw std::runtime_error("failed to initialize end_scene");

	if (MH_CreateHook(reset_target, &reset::hook, reinterpret_cast<void**>(&reset_original)) != MH_OK)
		throw std::runtime_error("failed to initialize reset");

	if (MH_CreateHook(lock_cursor_target, &lock_cursor::hook, reinterpret_cast<void**>(&lock_cursor_original)) != MH_OK)
		throw std::runtime_error("failed to initialize lock_cursor");

	if (MH_CreateHook(get_color_modulation_target, &get_color_modulation::hook, reinterpret_cast<void**>(&get_color_modulation_original)) != MH_OK)
		throw std::runtime_error("failed to initialize get_color_modulation");

	if (MH_CreateHook(is_using_static_prop_debug_modes_target, &is_using_static_prop_debug_modes::hook, reinterpret_cast<void**>(&is_using_static_prop_debug_modes_original)) != MH_OK)
		throw std::runtime_error("failed to initialize is_using_static_prop_debug_modes");

	if (MH_CreateHook(should_draw_fog_target, &should_draw_fog::hook, reinterpret_cast<void**>(&should_draw_fog_original)) != MH_OK)
		throw std::runtime_error("failed to initialize should_draw_fog");

	if (MH_CreateHook(should_skip_animation_frame_target, &should_skip_animation_frame::hook, reinterpret_cast<void**>(&should_skip_animation_frame_original)) != MH_OK)
		throw std::runtime_error("failed to initialize should_skip_animation_frame");

	if (MH_CreateHook(do_procedural_foot_plant_target, &do_procedural_foot_plant::hook, reinterpret_cast<void**>(&do_procedural_foot_plant_original)) != MH_OK)
		throw std::runtime_error("failed to initialize do_procedural_foot_plant");

	if (MH_CreateHook(build_transformations_target, &build_transformations::hook, reinterpret_cast<void**>(&build_transformations_original)) != MH_OK)
		throw std::runtime_error("failed to initialize build_transformations");

	if (MH_CreateHook(check_for_sequence_change_target, &check_for_sequence_change::hook, reinterpret_cast<void**>(&check_for_sequence_change_original)) != MH_OK)
		throw std::runtime_error("failed to initialize check_for_sequence_change");

	if (MH_CreateHook(is_hltv_target, &is_hltv::hook, reinterpret_cast<void**>(&is_hltv_original)) != MH_OK)
		throw std::runtime_error("failed to initialize is_hltv");

	if (MH_CreateHook(standard_blending_rules_target, &standard_blending_rules::hook, reinterpret_cast<void**>(&standard_blending_rules_original)) != MH_OK)
		throw std::runtime_error("failed to initialize standard_blending_rules");

	if (MH_CreateHook(calculate_view_target, &calculate_view::hook, reinterpret_cast<void**>(&calculate_view_original)) != MH_OK)
		throw std::runtime_error("failed to initialize calculate_view");

	if (MH_CreateHook(modify_eye_position_target, &modify_eye_position::hook, reinterpret_cast<void**>(&modify_eye_position_original)) != MH_OK)
		throw std::runtime_error("failed to initialize modify_eye_position");

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		throw std::runtime_error("failed to enable hooks");

	D3DDEVICE_CREATION_PARAMETERS params;
	interfaces::directx->GetCreationParameters(&params);
	window = params.hFocusWindow;
	menu::init(window, interfaces::directx);
	original_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(window, GWL_WNDPROC, reinterpret_cast<LONG>(wndproc::hook)));
	console::log("[setup] hooks initialized!\n");
	listener = new event_listener();
	return true;
}

void hooks::release() {
	SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG>(original_wndproc));
	delete listener;
	delete sequence_hook;
	MH_Uninitialize();
	MH_DisableHook(MH_ALL_HOOKS);
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

bool __stdcall hooks::create_move::hook(float input_sample_frametime, c_usercmd* cmd) {
	if (!cmd || !cmd->command_number)
		return create_move_original(input_sample_frametime, cmd);

	csgo::local_player = static_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	const volatile auto vlBaseAddress = *reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(_AddressOfReturnAddress()) - sizeof(std::uintptr_t));
	bool& send_packet = *reinterpret_cast<bool*>(vlBaseAddress - 0x1C);

	auto old_viewangles = cmd->viewangles;
	auto old_forwardmove = cmd->forwardmove;
	auto old_sidemove = cmd->sidemove;
	csgo::want_to_shoot = cmd->buttons & in_attack;
	
	features::bunny_hop(cmd);
	features::no_duck_delay(cmd);
	features::clan_tag_changer(cmd);
	features::reveal_ranks(cmd);
	prediction::start(cmd);
	features::aimbot(cmd);
	features::fake_lag(send_packet);
	features::legit_aimbot(cmd);
	features::trigger(cmd);
	features::anti_aim(cmd);
	prediction::end();
	features::auto_switch(cmd);

	math::correct_movement(old_viewangles, cmd, old_forwardmove, old_sidemove);

	cmd->forwardmove = std::clamp(cmd->forwardmove, -450.0f, 450.0f);
	cmd->sidemove = std::clamp(cmd->sidemove, -450.0f, 450.0f);
	cmd->upmove = std::clamp(cmd->upmove, -320.0f, 320.0f);

	cmd->viewangles.normalize();
	cmd->viewangles.x = std::clamp(cmd->viewangles.x, -89.0f, 89.0f);
	cmd->viewangles.y = std::clamp(cmd->viewangles.y, -180.0f, 180.0f);
	cmd->viewangles.z = 0.0f;

	return false;
}

void __stdcall hooks::paint_traverse::hook(unsigned int panel, bool force_repaint, bool allow_force) {
	static auto water_mark = std::string("DarkToolX - beta v4.6 - UID: ") + std::to_string(csgo::user.uid);
	switch (fnv::hash(interfaces::panel->get_panel_name(panel))) {
	case fnv::hash("MatSystemTopPanel"):
		render::text(10, 10, render::fonts::watermark_font, water_mark, false, color::white(255));
		break;
	case fnv::hash("HudZoom"):
		features::damage_indicator();
		if (features::remove_scope())
			return;
		else
			features::crosshair();
		break;
	}
	paint_traverse_original(interfaces::panel, panel, force_repaint, allow_force);
}

void __stdcall hooks::override_view::hook(view_setup_t* view)
{
	features::fov(view);
	features::thirdperson();
	override_view_original(interfaces::clientmode, view);
}

int __stdcall hooks::do_post_screen_effects::hook(int a1)
{
	if (interfaces::engine->is_in_game())
	{
		features::glow();
		features::no_flash();
	}
	return do_post_screen_effects_original(interfaces::clientmode, a1);
}

void __stdcall hooks::frame_stage_notify::hook(int stage)
{
	if (interfaces::engine->is_in_game())
	{
		switch (stage)
		{
		case FRAME_START:
			break;
		case FRAME_NET_UPDATE_START:
			features::step_esp();
			break;
		case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
			features::skin_changer();
			break;
		case FRAME_NET_UPDATE_POSTDATAUPDATE_END:
			break;
		case FRAME_NET_UPDATE_END:
			break;
		case FRAME_RENDER_START:
			features::sky_box_changer();
			break;
		case FRAME_RENDER_END:
			break;
		}
	}
	frame_stage_notify_original(interfaces::client, stage);
}

int __fastcall hooks::sv_teamid_overhead_get_int::hook(uintptr_t ecx, uintptr_t edx)
{
	static auto ret_to_process_input = uintptr_t(utilities::pattern_scan("client.dll", "85 C0 74 0D E8 ? ? ? ?"));
	auto ret_addr = uintptr_t(_ReturnAddress());

	if (ret_addr != ret_to_process_input || !csgo::conf->visuals().enemy_overhead_info)
		return sv_teamid_overhead_get_int_original(ecx, edx);

	const auto base = *reinterpret_cast<uintptr_t*>((reinterpret_cast<uintptr_t>(_AddressOfReturnAddress()) - sizeof(uintptr_t)));
	auto& is_spectator = *(bool*)(base - 0x18A);
	is_spectator = true;

	static auto cl_teamid_overhead_maxdist_spec = interfaces::console->get_convar("cl_teamid_overhead_maxdist_spec");
	cl_teamid_overhead_maxdist_spec->set_value(INT_MAX);

	static auto cl_teamid_overhead_always = interfaces::console->get_convar("cl_teamid_overhead_mode");
	cl_teamid_overhead_always->set_value(2);

	return 1;
}

void __stdcall hooks::emit_sound::hook(void* filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample, float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch, const vec3_t* pOrigin, const vec3_t* pDirection, vec3_t* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int params)
{
	switch (fnv::hash(pSoundEntry)) {
	case fnv::hash("UIPanorama.popup_accept_match_beep"):
		features::auto_accept();
		break;
	}
	emit_sound_original(interfaces::engine_sound, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, flVolume, flAttenuation, nSeed, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity, params);
}

long __stdcall hooks::end_scene::hook(IDirect3DDevice9* device)
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	menu::render(csgo::menu::enabled, *csgo::conf, *csgo::kits);
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	return end_scene_original(device);
}

long __stdcall hooks::reset::hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const auto result = reset_original(device, params);
	ImGui_ImplDX9_CreateDeviceObjects();
	return result;
}

void __stdcall hooks::lock_cursor::hook()
{
	if (csgo::menu::enabled)
	{
		interfaces::surface->unlock_cursor();
		return;
	}
	return lock_cursor_original(interfaces::surface);
}

void __fastcall hooks::get_color_modulation::hook(i_material* mat, void* edx, float* r, float* g, float* b)
{
	get_color_modulation_original(mat, r, g, b);
	features::night_mode(mat, *r, *g, *b);
}

bool __stdcall hooks::is_using_static_prop_debug_modes::hook()
{
	return csgo::conf->view().night_mode;
}

bool __stdcall hooks::should_draw_fog::hook()
{
	if (*static_cast<uint32_t*>(_ReturnAddress()) != 0x6274C084)
		return should_draw_fog_original(interfaces::clientmode);
	return !csgo::conf->view().no_fog;
}

bool __fastcall hooks::should_skip_animation_frame::hook(void* this_pointer, void* edx)
{
	return false;
}

void __fastcall hooks::do_procedural_foot_plant::hook(void* this_pointer, void* edx, void* bone_to_world, void* left_foot_chain, void* right_foot_chain, void* pos)
{
	return;
}

void __fastcall hooks::build_transformations::hook(player_t* this_pointer, void* edx, void* hdr, void* pos, void* q, const void* camera_transform, int bone_mask, void* bone_computed)
{
	this_pointer->is_jiggle_bones_enabled() = false;
	return build_transformations_original(this_pointer, edx, hdr, pos, q, camera_transform, bone_mask, bone_computed);
}

void __fastcall hooks::check_for_sequence_change::hook(void* this_pointer, void* edx, void* hdr, int cur_sequence, bool force_new_sequence, bool interpolate)
{
	return check_for_sequence_change_original(this_pointer, edx, hdr, cur_sequence, force_new_sequence, false);
}

bool __fastcall hooks::is_hltv::hook(void* this_pointer, void* edx)
{
	const auto ret_addr = uintptr_t(_ReturnAddress());
	static auto setup_velocity = uintptr_t(utilities::pattern_scan("client.dll", "84 C0 75 38 8B 0D ? ? ? ? 8B 01 8B 80"));
	if (ret_addr == setup_velocity)
		return true;

	static auto accumulate_layers = uintptr_t(utilities::pattern_scan("client.dll", "84 C0 75 0D F6 87"));
	if (ret_addr == accumulate_layers)
		return true;

	return is_hltv_original(this_pointer, edx);
}

void __fastcall hooks::standard_blending_rules::hook(player_t* this_pointer, void* edx, void* hdr, void* pos, void* q, float current_time, int bone_mask)
{
	if (!(this_pointer->get_effects() & 8))
		this_pointer->get_effects() |= 8;

	standard_blending_rules_original(this_pointer, edx, hdr, pos, q, current_time, bone_mask);

	this_pointer->get_effects() &= ~8;
}

void __fastcall hooks::calculate_view::hook(player_t* this_pointer, void* edx, vec3_t& eye_origin, vec3_t& eye_angles, float& z_near, float& z_far, float& fov)
{
	if (this_pointer != csgo::local_player)
		return calculate_view_original(this_pointer, edx, eye_origin, eye_angles, z_near, z_far, fov);

	const auto old_use_new_animation_state = this_pointer->use_new_animation_state();
	this_pointer->use_new_animation_state() = false;
	calculate_view_original(this_pointer, edx, eye_origin, eye_angles, z_near, z_far, fov);
	this_pointer->use_new_animation_state() = old_use_new_animation_state;
}

void __fastcall hooks::modify_eye_position::hook(anim_state* this_pointer, void* edx, vec3_t& input_eye_position)
{
	this_pointer->smooth_height_valid = false;
	return modify_eye_position_original(this_pointer, edx, input_eye_position);
}

void __cdecl hooks::sequence_proxy::hook(const c_recv_proxy_data* proxy_data_const, void* entity, void* output)
{
	static auto original_fn = sequence_hook->get_original_function();

	const auto proxy_data = const_cast<c_recv_proxy_data*>(proxy_data_const);
	const auto view_model = static_cast<base_view_model_t*>(entity);

	features::sequence_remapping(proxy_data, view_model);

	original_fn(proxy_data_const, entity, output);
}

LRESULT __stdcall hooks::wndproc::hook(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_KEYUP:
		if (wparam == VK_INSERT)
			csgo::menu::enabled = !csgo::menu::enabled;
		break;
	default:
		break;
	}

	if (csgo::menu::enabled)
		ImGui_ImplWin32_WndProcHandler(window, message, wparam, lparam);

	interfaces::inputsystem->enable_input(!csgo::menu::enabled);

	return CallWindowProc(original_wndproc, window, message, wparam, lparam);
}