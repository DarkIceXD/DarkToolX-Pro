#include "../utilities/csgo.hpp"

void math::correct_movement(c_usercmd* cmd, const float old_yaw) {
	const auto f1 = old_yaw < 0.f ? 360.0f + old_yaw : old_yaw;
	const auto f2 = cmd->viewangles.y < 0.0f ? 360.0f + cmd->viewangles.y : cmd->viewangles.y;
	const auto delta_view = 360.0f - ((f2 < f1) ? abs(f2 - f1) : (360.0f - abs(f1 - f2)));

	const auto old_forwardmove = cmd->forwardmove;
	const auto old_sidemove = cmd->sidemove;

	cmd->forwardmove = cos(DEG2RAD(delta_view)) * old_forwardmove + cos(DEG2RAD(delta_view + 90.f)) * old_sidemove;
	cmd->sidemove = sin(DEG2RAD(delta_view)) * old_forwardmove + sin(DEG2RAD(delta_view + 90.f)) * old_sidemove;
}

vec3_t math::calculate_angle(const vec3_t& a, const vec3_t& b) {

	const auto delta = a - b;
	vec3_t angles(atan(delta.z / delta.length_2d()) * M_RADPI, atan(delta.y / delta.x) * M_RADPI, 0.0f);
	if (delta.x >= 0.0)
		angles.y += 180.0f;
	return angles;
}

vec3_t math::transform_vector(const vec3_t& a, const matrix_t& b) {
	return {
		a.dot(b[0]) + b[0][3],
		a.dot(b[1]) + b[1][3],
		a.dot(b[2]) + b[2][3]
	};
}

vec3_t math::vector_angles(const vec3_t& forward) {
	vec3_t angles;

	if (forward.y == 0.0f && forward.x == 0.0f) {
		angles.x = (forward.z > 0.0f) ? 270.0f : 90.0f;
		angles.y = 0.0f;
	}
	else {
		angles.x = atan2(-forward.z, forward.length_2d()) * -180 / static_cast<float>(M_PI);
		angles.y = atan2(forward.y, forward.x) * 180 / static_cast<float>(M_PI);

		if (angles.y > 90)
			angles.y -= 180;
		else if (angles.y < 90)
			angles.y += 180;
		else if (angles.y == 90)
			angles.y = 0;
	}
	angles.z = 0.0f;

	return angles;
}

vec3_t math::vector_angles(const vec3_t& forward, const vec3_t& up)
{
	const auto left = vec3_t::cross_product(up, forward).normalized();

	const auto xy_dist = forward.length_2d();
	vec3_t angles;
	if (xy_dist > 0.001f)
	{
		angles.x = math::RAD2DEG(atan2f(-forward.z, xy_dist));
		angles.y = math::RAD2DEG(atan2f(forward.y, forward.x));
		const auto up_z = (left.y * forward.x) - (left.x * forward.y);
		angles.z = math::RAD2DEG(atan2f(left.z, up_z));
	}
	else
	{
		angles.x = math::RAD2DEG(atan2f(-forward.z, xy_dist));
		angles.y = math::RAD2DEG(atan2f(-left.x, left.y));
		angles.z = 0;
	}
	return angles;
}

vec3_t math::angle_vector(const vec3_t& angle) {
	const auto sp = sin(DEG2RAD(angle.x));
	const auto sy = sin(DEG2RAD(angle.y));
	const auto cp = cos(DEG2RAD(angle.x));
	const auto cy = cos(DEG2RAD(angle.y));

	return vec3_t(cp * cy, cp * sy, -sp);
}

void math::angle_vector(const vec3_t& angles, vec3_t* forward, vec3_t* right, vec3_t* up) {
	const auto sp = sin(DEG2RAD(angles.x));
	const auto sy = sin(DEG2RAD(angles.y));
	const auto cp = cos(DEG2RAD(angles.x));
	const auto cy = cos(DEG2RAD(angles.y));

	if (forward) {
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	const auto sr = sin(DEG2RAD(angles.z));
	const auto cr = cos(DEG2RAD(angles.z));
	if (right) {
		right->x = -1 * sr * sp * cy + -1 * cr * -sy;
		right->y = -1 * sr * sp * sy + -1 * cr * cy;
		right->z = -1 * sr * cp;
	}

	if (up) {
		up->x = cr * sp * cy + -sr * -sy;
		up->y = cr * sp * sy + -sr * cy;
		up->z = cr * cp;
	}
}

bool math::screen_transform(const vec3_t& point, vec3_t& screen) {
	auto matrix = interfaces::engine->world_to_screen_matrix();

	float w = matrix[3][0] * point.x + matrix[3][1] * point.y + matrix[3][2] * point.z + matrix[3][3];
	screen.x = matrix[0][0] * point.x + matrix[0][1] * point.y + matrix[0][2] * point.z + matrix[0][3];
	screen.y = matrix[1][0] * point.x + matrix[1][1] * point.y + matrix[1][2] * point.z + matrix[1][3];
	screen.z = 0.0f;

	int inverse_width = static_cast<int>((w < 0.001f) ? -1.0f / w :
		1.0f / w);

	screen.x *= inverse_width;
	screen.y *= inverse_width;
	return (w < 0.001f);
}

bool math::world_to_screen(const vec3_t& origin, vec2_t& screen) {
	static std::uintptr_t view_matrix;
	if (!view_matrix)
		view_matrix = *reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(utilities::pattern_scan("client.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9")) + 3) + 176;

	const auto& matrix = *reinterpret_cast<view_matrix_t*>(view_matrix);

	const auto w = matrix.m[3][0] * origin.x + matrix.m[3][1] * origin.y + matrix.m[3][2] * origin.z + matrix.m[3][3];
	if (w < 0.001f)
		return false;

	int x, y;
	interfaces::engine->get_screen_size(x, y);

	screen.x = static_cast<float>(x) / 2.0f;
	screen.y = static_cast<float>(y) / 2.0f;

	screen.x *= 1.0f + (matrix.m[0][0] * origin.x + matrix.m[0][1] * origin.y + matrix.m[0][2] * origin.z + matrix.m[0][3]) / w;
	screen.y *= 1.0f - (matrix.m[1][0] * origin.x + matrix.m[1][1] * origin.y + matrix.m[1][2] * origin.z + matrix.m[1][3]) / w;

	return true;
}

vec3_t math::vector_rotate(const vec3_t& in1, const matrix_t& in2)
{
	return { in1.dot(in2[0]), in1.dot(in2[1]), in1.dot(in2[2]) };
}

bool math::IntersectLineWithBB(const vec3_t& vStart, const vec3_t& vEndDelta, const vec3_t& vMin, const vec3_t& vMax)
{
	float d1, d2, f;
	bool startsolid = true;
	float t1 = -1.0f, t2 = 1.0f;

	for (int i = 0; i < 6; i++) {
		if (i >= 3) {
			int j = i - 3;

			d1 = vStart[j] - vMax[j];
			d2 = d1 + vEndDelta[j];
		}
		else {
			d1 = -vStart[i] + vMin[i];
			d2 = d1 - vEndDelta[i];
		}

		if (d1 > 0.0f && d2 > 0.0f)
			return false;
		else if (d1 <= 0.0f && d2 <= 0.0f)
			continue;

		if (d1 > 0)
			startsolid = false;

		if (d1 > d2) {
			f = d1;
			if (f < 0.0f)
				f = 0.0f;

			f /= d1 - d2;
			if (f > t1)
				t1 = f;
		}
		else {
			f = d1 / (d1 - d2);
			if (f < t2)
				t2 = f;
		}
	}

	return (startsolid || (t1 < t2&& t1 >= 0.0f));
}

bool math::IntersectLineWithOBB(const vec3_t& vStart, const vec3_t& vEndDelta, const vec3_t& vMin, const vec3_t& vMax, const matrix_t& matrix)
{
	const auto vStart2 = transform_vector(vStart, matrix);
	const auto vEndDelta2 = vector_rotate(vEndDelta, matrix);
	return IntersectLineWithBB(vStart2, vEndDelta2, vMin, vMax);
}

float math::fov(const vec3_t& a, const vec3_t& b) {
	const auto angles_delta = (b - a).normalized_angles();
	return sin(DEG2RAD(angles_delta.length()) / 2.0f) * 180.0f;
}