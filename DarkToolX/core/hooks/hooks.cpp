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
hooks::emit_sound::fn emit_sound_original = nullptr;
hooks::end_scene::fn end_scene_original = nullptr;
hooks::reset::fn reset_original = nullptr;
hooks::lock_cursor::fn lock_cursor_original = nullptr;
hooks::get_color_modulation::fn get_color_modulation_original = nullptr;
hooks::is_using_static_prop_debug_modes::fn is_using_static_prop_debug_modes_original = nullptr;
hooks::should_draw_fog::fn should_draw_fog_original = nullptr;
hooks::render_smoke_overlay::fn render_smoke_overlay_original = nullptr;
hooks::should_skip_animation_frame::fn should_skip_animation_frame_original = nullptr;
hooks::do_procedural_foot_plant::fn do_procedural_foot_plant_original = nullptr;
hooks::build_transformations::fn build_transformations_original = nullptr;
hooks::check_for_sequence_change::fn check_for_sequence_change_original = nullptr;
hooks::is_hltv::fn is_hltv_original = nullptr;
hooks::standard_blending_rules::fn standard_blending_rules_original = nullptr;
hooks::calculate_view::fn calculate_view_original = nullptr;
hooks::modify_eye_position::fn modify_eye_position_original = nullptr;
hooks::update_animation_state::fn update_animation_state_original = nullptr;
hooks::update_client_side_animation::fn update_client_side_animation_original = nullptr;
hooks::cl_grenadepreview::fn cl_grenadepreview_original = nullptr;
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
	const auto emit_sound_target = reinterpret_cast<void*>(get_virtual(interfaces::engine_sound, 5));
	const auto end_scene_target = reinterpret_cast<void*>(get_virtual(interfaces::directx, 42));
	const auto reset_target = reinterpret_cast<void*>(get_virtual(interfaces::directx, 16));
	const auto lock_cursor_target = reinterpret_cast<void*>(get_virtual(interfaces::surface, 67));
	const auto get_color_modulation_target = static_cast<void*>(utilities::pattern_scan("materialsystem.dll", "55 8B EC 83 EC ? 56 8B F1 8A 46"));
	const auto is_using_static_prop_debug_modes_target = static_cast<void*>(utilities::pattern_scan("engine.dll", "8B 0D ? ? ? ? 81 F9 ? ? ? ? 75 60 F3 0F 10 05 ? ? ? ? 0F 2E 05 ? ? ? ? 8B 15 ? ? ? ? 9F F6 C4 44 7A 1A 39 15 ? ? ? ? 75 12 A1 ? ? ? ? 33 05 ? ? ? ? A9 ? ? ? ? 74 1D 8B 0D ? ? ? ? 85 C9 74 19 8B 01 68 ? ? ? ? FF 90 ? ? ? ? 8B 15 ? ? ? ? 8B 0D ? ? ? ? 81 F2 ? ? ? ? EB 0D 8B 01 FF 50 34 8B 0D ? ? ? ? 8B D0 83 FA 01 0F"));
	const auto should_draw_fog_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 17));
	const auto render_smoke_overlay_target = reinterpret_cast<void*>(get_virtual(interfaces::view_render, 41));
	const auto should_skip_animation_frame_target = static_cast<void*>(utilities::pattern_scan("client.dll", "57 8B F9 8B 07 8B 80 ? ? ? ? FF D0 84 C0 75 02"));
	const auto do_procedural_foot_plant_target = static_cast<void*>(utilities::pattern_scan("client.dll", "55 8B EC 83 E4 F0 83 EC 78 56 8B F1 57 8B 56"));
	const auto build_transformations_target = static_cast<void*>(utilities::pattern_scan("client.dll", "81 EC ? ? ? ? 56 57 8B F9 8B 0D ? ? ? ? 89 7C 24 28") - 0x3);
	const auto check_for_sequence_change_target = static_cast<void*>(utilities::pattern_scan("client.dll", "55 8B EC 51 53 8B 5D 08 56 8B F1 57 85"));
	const auto is_hltv_target = reinterpret_cast<void*>(get_virtual(interfaces::engine, 93));
	const auto standard_blending_rules_target = static_cast<void*>(utilities::pattern_scan("client.dll", "55 8B EC 83 EC 08 8B 45 08 56 57 8B F9 8D"));
	const auto calculate_view_target = static_cast<void*>(utilities::pattern_scan("client.dll", "55 8B EC 83 EC 14 53 56 57 FF 75 18"));
	const auto modify_eye_position_target = static_cast<void*>(utilities::pattern_scan("client.dll", "55 8B EC 83 E4 F8 83 EC 70 56 57 8B F9 89 7C 24 14 83 7F 60"));
	const auto update_animation_state_target = static_cast<void*>(utilities::pattern_scan("client.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3"));
	const auto update_client_side_animation_target = static_cast<void*>(utilities::pattern_scan("client.dll", "55 8B EC 51 56 8B F1 80 BE ? ? ? ? ? 74"));
	const auto cl_grenadepreview_target = reinterpret_cast<void*>(get_virtual(interfaces::console->get_convar("cl_grenadepreview"), 13));
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

	if (MH_CreateHook(render_smoke_overlay_target, &render_smoke_overlay::hook, reinterpret_cast<void**>(&render_smoke_overlay_original)) != MH_OK)
		throw std::runtime_error("failed to initialize render_smoke_overlay");

	if (MH_CreateHook(should_skip_animation_frame_target, &should_skip_animation_frame::hook, reinterpret_cast<void**>(&should_skip_animation_frame_original)) != MH_OK)
		throw std::runtime_error("failed to initialize should_skip_animation_frame");

	if (MH_CreateHook(do_procedural_foot_plant_target, &do_procedural_foot_plant::hook, reinterpret_cast<void**>(&do_procedural_foot_plant_original)) != MH_OK)
		throw std::runtime_error("failed to initialize do_procedural_foot_plant");

	/*if (MH_CreateHook(build_transformations_target, &build_transformations::hook, reinterpret_cast<void**>(&build_transformations_original)) != MH_OK)
		throw std::runtime_error("failed to initialize build_transformations");*/

	if (MH_CreateHook(check_for_sequence_change_target, &check_for_sequence_change::hook, reinterpret_cast<void**>(&check_for_sequence_change_original)) != MH_OK)
		throw std::runtime_error("failed to initialize check_for_sequence_change");

	if (MH_CreateHook(is_hltv_target, &is_hltv::hook, reinterpret_cast<void**>(&is_hltv_original)) != MH_OK)
		throw std::runtime_error("failed to initialize is_hltv");

	/*if (MH_CreateHook(standard_blending_rules_target, &standard_blending_rules::hook, reinterpret_cast<void**>(&standard_blending_rules_original)) != MH_OK)
		throw std::runtime_error("failed to initialize standard_blending_rules");*/

	if (MH_CreateHook(calculate_view_target, &calculate_view::hook, reinterpret_cast<void**>(&calculate_view_original)) != MH_OK)
		throw std::runtime_error("failed to initialize calculate_view");

	/*if (MH_CreateHook(modify_eye_position_target, &modify_eye_position::hook, reinterpret_cast<void**>(&modify_eye_position_original)) != MH_OK)
		throw std::runtime_error("failed to initialize modify_eye_position");*/

	if (MH_CreateHook(update_animation_state_target, &update_animation_state::hook, reinterpret_cast<void**>(&update_animation_state_original)) != MH_OK)
		throw std::runtime_error("failed to initialize update_animation_state");

	if (MH_CreateHook(update_client_side_animation_target, &update_client_side_animation::hook, reinterpret_cast<void**>(&update_client_side_animation_original)) != MH_OK)
		throw std::runtime_error("failed to initialize update_client_side_animation");

	/*if (MH_CreateHook(cl_grenadepreview_target, &cl_grenadepreview::hook, reinterpret_cast<void**>(&cl_grenadepreview_original)) != MH_OK)
		throw std::runtime_error("failed to initialize cl_grenadepreview");*/
	D3DDEVICE_CREATION_PARAMETERS params;
	interfaces::directx->GetCreationParameters(&params);
	window = params.hFocusWindow;
	menu::init(window, interfaces::directx);
	original_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(window, GWL_WNDPROC, reinterpret_cast<LONG>(wndproc::hook)));

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		throw std::runtime_error("failed to enable hooks");
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
}

bool __stdcall hooks::create_move::hook(float input_sample_frametime, c_usercmd* cmd) {
	create_move_original(input_sample_frametime, cmd);
	if (!cmd || !cmd->command_number)
		return false;

	/*uintptr_t* frame_pointer;
	__asm mov frame_pointer, ebp;
	bool& send_packet = *reinterpret_cast<bool*>(*frame_pointer - 0x1C);*/
	bool send_packet = true;

	const auto old_yaw = cmd->viewangles.y;
	csgo::want_to_shoot = csgo::manual_shoot = cmd->buttons & in_attack;
	csgo::target = {};
	if (csgo::didnt_shoot_due_to_hitchance || (cmd->forwardmove == 0 && cmd->sidemove == 0))
		features::util::auto_stop(cmd);
	csgo::didnt_shoot_due_to_hitchance = false;
	features::bunny_hop(cmd);
	features::no_duck_delay(cmd);
	features::reveal_ranks(cmd);
	prediction::start(cmd);
	{
		features::clan_tag_changer();
		features::aimbot::run(cmd);
		features::trigger(cmd);
		features::backtrack::run(cmd);
		features::fake_lag(send_packet);
		features::fake_duck(cmd, send_packet);
		features::anti_aim(cmd, send_packet);
		features::dormant();
		features::slow_walk(cmd);
	}
	prediction::end();
	features::quick_switch(cmd);
	features::resolver::new_tick(cmd);
	math::correct_movement(cmd, old_yaw);

	cmd->forwardmove = std::clamp(cmd->forwardmove, -450.0f, 450.0f);
	cmd->sidemove = std::clamp(cmd->sidemove, -450.0f, 450.0f);
	cmd->upmove = std::clamp(cmd->upmove, -320.0f, 320.0f);

	cmd->viewangles.normalize_angles();
	cmd->viewangles.x = std::clamp(cmd->viewangles.x, -89.0f, 89.0f);
	cmd->viewangles.y = std::clamp(cmd->viewangles.y, -180.0f, 180.0f);
	cmd->viewangles.z = 0.0f;

	csgo::real = cmd->viewangles;
	csgo::should_animate = true;

	return false;
}

void __stdcall hooks::paint_traverse::hook(unsigned int panel, bool force_repaint, bool allow_force) {
	switch (fnv::hash(interfaces::panel->get_panel_name(panel))) {
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
	features::glow();
	features::no_flash();
	return do_post_screen_effects_original(interfaces::clientmode, a1);
}

void __stdcall hooks::frame_stage_notify::hook(int stage)
{
	static auto get_local_player = *reinterpret_cast<player_t***>(utilities::pattern_scan("client.dll", "A1 ? ? ? ? 89 45 BC 85 C0") + 1);
	csgo::local_player = *get_local_player;
	if (csgo::local_player && interfaces::engine->is_in_game())
	{
		switch (stage)
		{
		case FRAME_START:
			features::esp::update();
			break;
		case FRAME_NET_UPDATE_START:
			break;
		case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
			features::resolver::run();
			features::skin_changer();
			break;
		case FRAME_NET_UPDATE_POSTDATAUPDATE_END:
			break;
		case FRAME_NET_UPDATE_END:
			break;
		case FRAME_RENDER_START:
			// features::animation_fix();
			features::modify_smoke();
			features::backtrack::update();
			features::sky_box_changer();
			features::step_esp::draw();
			features::bullet_tracers::draw();
			break;
		case FRAME_RENDER_END:
			break;
		}
	}
	frame_stage_notify_original(interfaces::client, stage);
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
	static auto water_mark = std::format("DarkToolX Pro - beta v16.1 - UID: {}", csgo::user.user.uid);
	IDirect3DStateBlock9* pixel_state = NULL;
	device->CreateStateBlock(D3DSBT_ALL, &pixel_state);
	pixel_state->Capture();
	device->SetVertexShader(nullptr);
	device->SetPixelShader(nullptr);
	device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	device->SetRenderState(D3DRS_FOGENABLE, FALSE);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
	device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
	device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	auto draw_list = ImGui::GetBackgroundDrawList();
	draw_list->AddText({ 5, 5 }, IM_COL32_WHITE, water_mark.c_str());
	features::esp::draw(draw_list);
	menu::render(csgo::menu::enabled, csgo::cfg);
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	pixel_state->Apply();
	pixel_state->Release();
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
	switch (fnv::hash(mat->get_texture_group_name()))
	{
	case fnv::hash("StaticProp textures"):
		features::color_modulation::night_mode::static_prop(*r, *g, *b);
		break;
	case fnv::hash("SkyBox textures"):
		features::color_modulation::night_mode::sky_box(*r, *g, *b);
		break;
	case fnv::hash("World textures"):
		features::color_modulation::night_mode::world(*r, *g, *b);
		features::color_modulation::asus_walls::world(mat);
		break;
	}
}

bool __stdcall hooks::is_using_static_prop_debug_modes::hook()
{
	return csgo::cfg.view().night_mode;
}

bool __stdcall hooks::should_draw_fog::hook()
{
	if (*static_cast<uint32_t*>(_ReturnAddress()) != 0x6274C084)
		return should_draw_fog_original(interfaces::clientmode);
	return !csgo::cfg.view().no_fog;
}

void __stdcall hooks::render_smoke_overlay::hook(bool update)
{
	if (csgo::cfg.view().modify_smoke)
		*reinterpret_cast<float*>(std::uintptr_t(interfaces::view_render) + 0x588) = 0.0f;
	else
		render_smoke_overlay_original(interfaces::view_render, update);
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
	return build_transformations_original(this_pointer, hdr, pos, q, camera_transform, bone_mask, bone_computed);
}

void __fastcall hooks::check_for_sequence_change::hook(void* this_pointer, void* edx, void* hdr, int cur_sequence, bool force_new_sequence, bool interpolate)
{
	return check_for_sequence_change_original(this_pointer, hdr, cur_sequence, force_new_sequence, false);
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

	return is_hltv_original(this_pointer);
}

void __fastcall hooks::standard_blending_rules::hook(player_t* this_pointer, void* edx, void* hdr, void* pos, void* q, float current_time, int bone_mask)
{
	if (!(this_pointer->get_effects() & 8))
		this_pointer->get_effects() |= 8;

	standard_blending_rules_original(this_pointer, hdr, pos, q, current_time, bone_mask);

	this_pointer->get_effects() &= ~8;
}

void __fastcall hooks::calculate_view::hook(player_t* this_pointer, void* edx, vec3_t& eye_origin, vec3_t& eye_angles, float& z_near, float& z_far, float& fov)
{
	if (this_pointer != csgo::local_player || !this_pointer->is_alive())
		return calculate_view_original(this_pointer, eye_origin, eye_angles, z_near, z_far, fov);

	const auto old = this_pointer->use_new_animation_state();
	this_pointer->use_new_animation_state() = false;
	calculate_view_original(this_pointer, eye_origin, eye_angles, z_near, z_far, fov);
	this_pointer->use_new_animation_state() = old;
}

void __fastcall hooks::modify_eye_position::hook(anim_state* this_pointer, void* edx, vec3_t& input_eye_position)
{
	this_pointer->smooth_height_valid = false;
	return modify_eye_position_original(this_pointer, input_eye_position);
}

void __vectorcall hooks::update_animation_state::hook(anim_state* this_pointer, void* unknown, float z, float y, float x, void* unknown1)
{
	if (this_pointer->last_clientside_anim_update_framecount == interfaces::globals->frame_count)
		this_pointer->last_clientside_anim_update_framecount -= 1;
	if (this_pointer->entity != csgo::local_player)
		return update_animation_state_original(this_pointer, unknown, z, y, x, unknown1);

	return update_animation_state_original(this_pointer, unknown, z, csgo::real.y, csgo::real.x, unknown1);
}

void __fastcall hooks::update_client_side_animation::hook(player_t* this_pointer, void* edx)
{
	if (this_pointer != csgo::local_player)
		return update_client_side_animation_original(this_pointer);

	if (csgo::should_animate)
		update_client_side_animation_original(this_pointer);
}

bool __fastcall hooks::cl_grenadepreview::hook(void* this_pointer, void* edx)
{
	static auto cl_grenadepreview = reinterpret_cast<void*>(utilities::pattern_scan("client.dll", "85 C0 74 7D 83 BF"));
	if (cl_grenadepreview == _ReturnAddress())
		return 1;

	return cl_grenadepreview_original(this_pointer, edx);
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