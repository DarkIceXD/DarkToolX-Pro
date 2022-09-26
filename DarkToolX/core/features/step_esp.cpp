#include "features.hpp"

static std::vector<vec3_t> steps;

static void draw_circle(const vec3_t& position, const config::rgb& rgb, const float max_radius, const float lifetime, const float width)
{
	BeamInfo_t beam_info;
	beam_info.m_nType = TE_BEAMRINGPOINT;
	beam_info.m_pszModelName = "sprites/physbeam.vmt";
	beam_info.m_nModelIndex = interfaces::model_info->get_model_index(beam_info.m_pszModelName);
	beam_info.m_flHaloScale = 5;
	beam_info.m_flLife = lifetime;
	beam_info.m_flWidth = width;
	beam_info.m_flRed = rgb.r * 255;
	beam_info.m_flGreen = rgb.g * 255;
	beam_info.m_flBlue = rgb.b * 255;
	beam_info.m_flBrightness = rgb.a * 255;
	beam_info.m_flFrameRate = 60.f;
	beam_info.m_vecCenter = position + vec3_t(0, 0, 5);
	beam_info.m_flStartRadius = 10;
	beam_info.m_flEndRadius = max_radius;

	auto beam = interfaces::render_beams->create_beam_ring_point(beam_info);
	if (beam)
		interfaces::render_beams->draw_beam(beam);
}

void features::step_esp::player_footstep(i_game_event* event)
{
	if (!csgo::cfg.visuals().step_color.enabled)
		return;

	auto walker = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_player_for_user_id(event->get_int("userid"))));
	if (!walker || walker->dormant())
		return;

	if (!csgo::local_player || !csgo::local_player->is_enemy(walker))
		return;

	steps.push_back(walker->abs_origin());
}

void features::step_esp::draw()
{
	for (const auto& step : steps)
		draw_circle(step, csgo::cfg.visuals().step_color, csgo::cfg.visuals().step_max_radius, csgo::cfg.visuals().step_lifetime, csgo::cfg.visuals().step_width);
	steps.clear();
}