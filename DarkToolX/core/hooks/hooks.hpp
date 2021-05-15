#pragma once

namespace hooks {
	bool initialize();
	void release();

	inline unsigned int get_virtual(void* _class, unsigned int index) { return static_cast<unsigned int>((*static_cast<int**>(_class))[index]); }

	namespace create_move {
		using fn = bool(__stdcall*)(float, c_usercmd*);
		bool __stdcall hook(float input_sample_frametime, c_usercmd* cmd);
	};

	namespace paint_traverse {
		using fn = void(__thiscall*)(i_panel*, unsigned int, bool, bool);
		void __stdcall hook(unsigned int panel, bool force_repaint, bool allow_force);
	}

	namespace override_view {
		using fn = bool(__thiscall*)(i_client_mode*, view_setup_t*);
		void __stdcall hook(view_setup_t* setup);
	};

	namespace do_post_screen_effects {
		using fn = int(__thiscall*)(i_client_mode*, int);
		int __stdcall hook(int a1);
	};

	namespace frame_stage_notify {
		using fn = void(__thiscall*)(i_base_client_dll*, int);
		void __stdcall hook(int stage);
	};

	namespace emit_sound {
		using fn = void(__thiscall*)(i_engine_sound*, void*, int, int, const char*, unsigned int, const char*, float, float, int, int, int, const vec3_t*, const vec3_t*, vec3_t*, bool, float, int, int);
		void __stdcall hook(void* filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample, float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch, const vec3_t* pOrigin, const vec3_t* pDirection, vec3_t* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int params);
	};

	namespace end_scene {
		using fn = long(__stdcall*)(IDirect3DDevice9*);
		long __stdcall hook(IDirect3DDevice9* device);
	};

	namespace reset {
		using fn = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
		long __stdcall hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params);
	};

	namespace lock_cursor {
		using fn = void(__thiscall*)(i_surface*);
		void __stdcall hook();
	};

	namespace get_color_modulation {
		using fn = void(__thiscall*)(void*, float*, float*, float*);
		void __fastcall hook(i_material* mat, void* edx, float* r, float* g, float* b);
	};

	namespace is_using_static_prop_debug_modes {
		using fn = void(__fastcall*)();
		bool __stdcall hook();
	};

	namespace should_draw_fog {
		using fn = bool(__thiscall*)(i_client_mode*);
		bool __stdcall hook();
	};

	namespace render_smoke_overlay {
		using fn = void(__thiscall*)(void* this_pointer, bool update);
		void __stdcall hook(bool update);
	};

	namespace should_skip_animation_frame {
		using fn = bool(__fastcall*)(void*, void*);
		bool __fastcall hook(void* this_pointer, void* edx);
	};

	namespace do_procedural_foot_plant {
		using fn = void(__fastcall*)(void*, void*, void*, void*, void*, void*);
		void __fastcall hook(void* this_pointer, void* edx, void* bone_to_world, void* left_foot_chain, void* right_foot_chain, void* pos);
	};

	namespace build_transformations {
		using fn = void(__thiscall*)(player_t*, void*, void*, void*, const void*, int, void*);
		void __fastcall hook(player_t* this_pointer, void* edx, void* hdr, void* pos, void* q, const void* camera_transform, int bone_mask, void* bone_computed);
	};

	namespace check_for_sequence_change {
		using fn = void(__thiscall*)(void*, void*, int, bool, bool);
		void __fastcall hook(void* this_pointer, void* edx, void* hdr, int cur_sequence, bool force_new_sequence, bool interpolate);
	};

	namespace is_hltv {
		using fn = bool(__thiscall*)(void*);
		bool __fastcall hook(void* this_pointer, void* edx);
	};

	namespace standard_blending_rules {
		using fn = void(__thiscall*)(player_t*, void*, void*, void*, float, int);
		void __fastcall hook(player_t* this_pointer, void* edx, void* hdr, void* pos, void* q, float current_time, int bone_mask);
	};

	namespace calculate_view {
		using fn = void(__thiscall*)(player_t*, vec3_t&, vec3_t&, float&, float&, float&);
		void __fastcall hook(player_t* this_pointer, void* edx, vec3_t& eye_origin, vec3_t& eye_angles, float& z_near, float& z_far, float& fov);
	};

	namespace modify_eye_position {
		using fn = void(__thiscall*)(anim_state*, vec3_t&);
		void __fastcall hook(anim_state* this_pointer, void* edx, vec3_t& input_eye_position);
	};

	namespace update_animation_state {
		using fn = void(__vectorcall*)(anim_state*, void*, float, float, float, void*);
		void __vectorcall hook(anim_state* this_pointer, void* unknown, float z, float y, float x, void* unknown1);
	};

	namespace update_client_side_animation {
		using fn = void(__thiscall*)(player_t*);
		void __fastcall hook(player_t* this_pointer, void* edx);
	};

	namespace cl_grenadepreview {
		using fn = bool(__fastcall*)(void*, void*);
		bool __fastcall hook(void* this_pointer, void* edx);
	};

	namespace sequence_proxy {
		void __cdecl hook(const c_recv_proxy_data* proxy_data_const, void* entity, void* output);
	};

	namespace wndproc {
		LRESULT __stdcall hook(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
	};
}
