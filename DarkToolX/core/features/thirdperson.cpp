#include "features.hpp"

void features::thirdperson() {
	if (!csgo::local_player)
		return;

	if (csgo::local_player->health() > 0) {
		if (csgo::conf->view().key_bind_type == 1 && GetAsyncKeyState(csgo::conf->view().key_bind) & 1)
			csgo::conf->view().thirdperson = !csgo::conf->view().thirdperson;
		else if (csgo::conf->view().key_bind_type == 2 && !GetAsyncKeyState(csgo::conf->view().key_bind))
			return;
		if (csgo::conf->view().thirdperson) {
			if (interfaces::input->camera_in_third_person)
				return;

			float range = csgo::conf->view().range;
			vec3_t angles;
			interfaces::engine->get_view_angles(angles);
			angles.z = 0.f;
			auto forward(math::angle_vector(angles));
			auto eye_pos(csgo::local_player->get_eye_pos());

			vec3_t max_vec(16.f, 16.f, 16.f);
			auto vec = eye_pos - forward * range;
			ray_t ray(eye_pos, vec, max_vec * -1, max_vec);
			trace_filter filter(csgo::local_player);
			trace_t trace;

			interfaces::trace_ray->trace_ray(ray, MASK_SOLID, &filter, &trace);

			if (trace.flFraction < 1.f)
				range *= trace.flFraction;

			angles.z = range;
			interfaces::input->camera_in_third_person = true;
			interfaces::input->camera_offset = angles;
			interfaces::engine->execute_cmd("cl_updatevisibility");
		}
		else
		{
			interfaces::input->camera_in_third_person = false;
		}
	}
	else
	{
		if (csgo::conf->view().spectator_thirdperson)
		{
			auto& mode = csgo::local_player->observer_mode();
			if (mode != 5)
				mode = 5;
		}
	}
}