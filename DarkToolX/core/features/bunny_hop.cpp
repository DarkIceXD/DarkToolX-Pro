#include "features.hpp"

static float angleDiffRad(const float a1, const float a2)
{
	float delta = std::remainder(a1 - a2, math::M_PI * 2);
	if (a1 > a2)
	{
		if (delta >= math::M_PI)
			delta -= math::M_PI * 2;
	}
	else
	{
		if (delta <= -math::M_PI)
			delta += math::M_PI * 2;
	}
	return delta;
}

static float idealAngleDelta2(const float speed)
{
	if (!csgo::local_player) return 0.0f;
	const auto activeWeapon = csgo::local_player->active_weapon();
	if (!activeWeapon) return 0.0f;
	const auto weaponData = activeWeapon->get_weapon_data();
	if (!weaponData) return 0.0f;

	float steer = 30;
	steer *= csgo::local_player->is_scoped() ? weaponData->max_speed_alt / 250 : weaponData->max_speed / 250;
	return std::fminf(std::asinf(steer / speed), math::M_PI / 2);
}

static float idealAngleDelta3(const float speed)
{
	static const auto maxWishSpeedVar = interfaces::console->get_convar("sv_air_max_wishspeed");
	return std::fminf(std::atanf(maxWishSpeedVar->get_float() / speed), math::M_PI / 2);
}

void auto_strafe(c_usercmd* cmd)
{
	if (~cmd->buttons & in_jump || csgo::local_player->flags() & fl_onground)
		return;

	const float curSpeed = csgo::local_player->velocity().length_2d();
	const float idealDelta = idealAngleDelta2(curSpeed);
	if (idealDelta == 0.0f)
		return;

	const float deltaAt90 = idealAngleDelta3(curSpeed);
	const float yawRad = math::DEG2RAD(cmd->viewangles.y);
	const float curVel = curSpeed != 0.0f ? std::atan2f(csgo::local_player->velocity().y, csgo::local_player->velocity().x) - yawRad : 0.0f;
	const float wishAng = std::atan2f(-cmd->sidemove, cmd->forwardmove);
	const float angleDelta = angleDiffRad(curVel, wishAng);
	const float move = angleDelta < 0.0f ? curVel + math::M_PI / 2 - deltaAt90 + idealDelta : curVel - math::M_PI / 2 + deltaAt90 - idealDelta;
	cmd->forwardmove = std::cosf(move) * 450.0f;
	cmd->sidemove = -std::sinf(move) * 450.0f;
}

void features::bunny_hop(c_usercmd* cmd)
{
	if (!csgo::conf->misc().bhop)
		return;

	if (!csgo::local_player)
		return;

	const int move_type = csgo::local_player->move_type();
	if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer)
		return;

	if (csgo::conf->misc().auto_strafe)
		auto_strafe(cmd);

	if (!(csgo::local_player->flags() & fl_onground))
		cmd->buttons &= ~in_jump;
}