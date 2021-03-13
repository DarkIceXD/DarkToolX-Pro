#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../../dependencies/imgui/imgui.h"

struct auto_wall_data {
	player_t* entity;
	float damage;
};

namespace features {
	void aimbot(c_usercmd* cmd);
	void animation_fix();
	void anti_aim(c_usercmd* cmd, bool& send_packet);
	void auto_accept();
	void auto_shoot(c_usercmd* cmd, weapon_t* weapon, const weapon_info_t* weapon_data);
	void auto_switch(c_usercmd* cmd);
	auto_wall_data auto_wall(vec3_t direction, const weapon_info_t* weapon_data, const bool bangable);
	void bullet_tracers();
	void bunny_hop(c_usercmd* cmd);
	bool can_force_item_update(weapon_t* item);
	void clan_tag_changer(const c_usercmd* cmd);
	void crosshair();
	void damage_indicator();
	void dormant();
	void esp(ImDrawList* draw_list);
	void fake_lag(bool& send_packet);
	void force_item_update(weapon_t* item);
	void fov(view_setup_t* view);
	void glow();
	bool hitchance(const vec3_t& view_angles, player_t* player, const int needed_hitchance, weapon_t* weapon, const float range);
	void legit_aimbot(c_usercmd* cmd);
	void night_mode(i_material* mat, float& r, float& g, float& b);
	void no_duck_delay(c_usercmd* cmd);
	void no_flash();
	void ragdolls();
	bool remove_scope();
	void reveal_ranks(const c_usercmd* cmd);
	void sequence_remapping(c_recv_proxy_data* data, base_view_model_t* entity);
	void skin_changer();
	void sky_box_changer();
	void slow_walk(c_usercmd* cmd, float& forwardmove, float& sidemove);
	void step_esp();
	void thirdperson();
	void trigger(c_usercmd* cmd);
	namespace backtrack {
		int time_to_ticks(const float time);
		float get_lerp();
		bool valid(const float simtime);
		void update();
		void run(c_usercmd* cmd);
	}
	namespace events {
		void hit_sound(i_game_event* event);
		void update_stattrak(i_game_event* event);
		void step_esp(i_game_event* event);
		void chat_bot(i_game_event* event);
		void chat_listener(i_game_event* event);
		void bullet_tracers(i_game_event* event);
	}
}