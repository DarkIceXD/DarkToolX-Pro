#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../../dependencies/imgui/imgui.h"
#include <deque>

struct auto_wall_data {
	player_t* entity;
	float damage;
};

struct record {
	vec3_t origin, mins, maxs, eye;
	float simulation_time;
	matrix_t matrix[256];
};

namespace features {
	void animation_fix();
	void anti_aim(c_usercmd* cmd, bool& send_packet);
	void auto_accept();
	void quick_switch(c_usercmd* cmd);
	void bunny_hop(c_usercmd* cmd);
	void clan_tag_changer();
	void crosshair();
	void damage_indicator();
	void dormant();
	void fake_duck(c_usercmd* cmd, bool& send_packet);
	void fake_lag(bool& send_packet);
	void fov(view_setup_t* view);
	void glow();
	void modify_smoke();
	void no_duck_delay(c_usercmd* cmd);
	void no_flash();
	bool remove_scope();
	void reveal_ranks(const c_usercmd* cmd);
	void sequence_remapping(c_recv_proxy_data* data, base_view_model_t* entity);
	void skin_changer();
	void sky_box_changer();
	void slow_walk(c_usercmd* cmd);
	void thirdperson();
	void trigger(c_usercmd* cmd);
	namespace aimbot {
		void run(c_usercmd* cmd);
		void silent(c_usercmd* cmd, weapon_t* weapon, const weapon_info_t* weapon_data);
		void legit(c_usercmd* cmd, weapon_t* weapon, const weapon_info_t* weapon_data);
	}
	namespace backtrack {
		std::deque<record>& get_records(const int player_index);
		void restore_record(player_t* entity, const int record_index);
		bool valid(const float simtime);
		int restore_tick_count(const int player_index, const int record_index);
		void update();
		void run(c_usercmd* cmd);
	}
	namespace bullet_tracers {
		void bullet_impact(i_game_event* event);
		void draw();
	}
	namespace esp {
		void update();
		void draw(ImDrawList* draw_list);
	}
	namespace logs {
		void player_hurt(i_game_event* event);
		void player_death(i_game_event* event);
		void vote_cast(i_game_event* event);
		void item_pickup(i_game_event* event);
	}
	namespace resolver {
		void new_tick(c_usercmd* cmd);
		void run();
		void weapon_fire(i_game_event* event);
		void player_hurt(i_game_event* event);
	}
	namespace step_esp{
		void player_footstep(i_game_event* event);
		void draw();
	}
	namespace util {
		void auto_shoot(c_usercmd* cmd, weapon_t* weapon, const weapon_info_t* weapon_data);
		void auto_stop(c_usercmd* cmd);
		auto_wall_data auto_wall(vec3_t direction, const weapon_info_t* weapon_data, const bool bangable);
		bool cock_revolver(weapon_t* weapon);
		bool can_force_item_update(weapon_t* item);
		void force_item_update(weapon_t* item);
		bool hitchance(const vec3_t& view_angles, player_t* player, const int needed_hitchance, weapon_t* weapon, const float range);
		void walkbot(c_usercmd* cmd);
	}
	namespace color_modulation {
		namespace night_mode {
			void static_prop(float& r, float& g, float& b);
			void sky_box(float& r, float& g, float& b);
			void world(float& r, float& g, float& b);
		}
		namespace asus_walls {
			void world(i_material* mat);
		}
	};
	namespace events {
		void hit_sound(i_game_event* event);
		void update_stattrak(i_game_event* event);
		void chat_bot(i_game_event* event);
	}
}