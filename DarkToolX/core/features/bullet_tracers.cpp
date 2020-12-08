#include "features.hpp"

struct bullet_data {
	vec3_t start, end;
	bool enemy;
};

static std::vector<bullet_data> bullets;

static void draw_trace(const bullet_data& bullet, const config::rgb& rgb, const float lifetime)
{
	BeamInfo_t beam_info;
	beam_info.m_pszModelName = "sprites/physbeam.vmt";
	beam_info.m_nModelIndex = interfaces::model_info->get_model_index("sprites/physbeam.vmt");
	beam_info.m_flHaloScale = 0.0f;
	beam_info.m_vecStart = bullet.start;
	beam_info.m_vecEnd = bullet.end;
	beam_info.m_flLife = lifetime;
	beam_info.m_flFadeLength = 0.1f;
	beam_info.m_flAmplitude = 2.3f;
	beam_info.m_nSegments = 2;
	beam_info.m_flRed = rgb.r * 255;
	beam_info.m_flGreen = rgb.g * 255;
	beam_info.m_flBlue = rgb.b * 255;
	beam_info.m_flBrightness = rgb.a * 255;
	beam_info.m_flBrightness = 255.f;
	beam_info.m_flSpeed = 0.2f;
	beam_info.m_nStartFrame = 0;
	beam_info.m_flFrameRate = 0.f;
	beam_info.m_flWidth = 3.5f;
	beam_info.m_flEndWidth = 1.1f;

	auto beam = interfaces::render_beams->create_beam_point(beam_info);
	if (beam)
		interfaces::render_beams->draw_beam(beam);
}

void features::events::bullet_tracers(i_game_event* event)
{
	auto shooting_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_player_for_user_id(event->get_int("userid"))));
	if (!shooting_player || shooting_player->dormant())
		return;

	if (!csgo::local_player)
		return;

	const auto is_enemy = csgo::local_player->is_enemy(shooting_player);
	const auto enabled = is_enemy ? csgo::conf->visuals().bullet_trace_enemy.enabled : (shooting_player == csgo::local_player ? csgo::conf->visuals().bullet_trace_local_player : csgo::conf->visuals().bullet_trace_team.enabled);
	if (!enabled)
		return;

	bullets.push_back({ shooting_player->get_eye_pos(), { event->get_float("x"), event->get_float("y"), event->get_float("z") }, is_enemy });
}

void features::bullet_tracers()
{
	for (const auto& bullet : bullets)
		draw_trace(bullet, bullet.enemy ? csgo::conf->visuals().bullet_trace_enemy : csgo::conf->visuals().bullet_trace_team, csgo::conf->visuals().bullet_trace_lifetime);
	bullets.clear();
}