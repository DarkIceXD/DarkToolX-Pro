#pragma once

class anim_layer
{
public:
	char  pad_0000[20];
	// These should also be present in the padding, don't see the use for it though
	//float	m_flLayerAnimtime;
	//float	m_flLayerFadeOuttime;
	uint32_t m_nOrder; //0x0014
	uint32_t m_nSequence; //0x0018
	float_t m_flPrevCycle; //0x001C
	float_t m_flWeight; //0x0020
	float_t m_flWeightDeltaRate; //0x0024
	float_t m_flPlaybackRate; //0x0028
	float_t m_flCycle; //0x002C
	void* m_pOwner; //0x0030 // player's thisptr
	char  pad_0038[4]; //0x0034
};

class anim_state {
public:
	char	u0[ 0x18 ];
	float	anim_update_timer;
	char	u1[ 0xC ];
	float	started_moving_time;
	float	last_move_time;
	char	u2[ 0x10 ];
	float	last_lby_time;
	char	u3[ 0x8 ];
	float	run_amount;
	char	u4[ 0x10 ];
	void	*entity;
	__int32 active_weapon;
	__int32 last_active_weapon;
	float	last_clientside_anim_update_time;
	__int32 last_clientside_anim_update_framecount;
	float	eye_timer;
	float	eye_angles_y;
	float	eye_angles_x;
	float	goal_feet_yaw;
	float	current_feet_yaw;
	float	torso_yaw;
	float	last_move_yaw;
	float	lean_amount;
	char	u5[ 0x4 ];
	float	feet_cycle;
	float	feet_yaw_rate;
	char	u6[ 0x4 ];
	float	duck_amount;
	float	landing_duck_amount;
	char	u7[ 0x4 ];
	vec3_t	current_origin;
	vec3_t	last_origin;
	float	velocity_x;
	float	velocity_y;
	char	u8[ 0x10 ];
	float	move_direction_1;
	float	move_direction_2;
	char	u9[ 0x4 ];
	float	velocity;
	float	jump_fall_velocity; 
	float	clamped_velocity; 
	float	feet_speed_forwards_or_sideways;
	float	feet_speed_unknown_forwards_or_sideways; 
	float	last_time_started_moving;
	float	last_time_stopped_moving;
	bool	on_ground;
	bool	hit_in_ground_anim;
	char	u10[ 0x4 ];
	float	last_origin_z;
	float	head_from_ground_distance_standing;
	float	stop_to_full_running_fraction;
	char	u11[ 0x14 ];
	__int32 is_not_moving;
	char	u12[ 0x20 ];
	float	last_anim_update_time;
	float	moving_direction_x;
	float	moving_direction_y;
	float	moving_direction_z;
	char	u13[ 0x44 ];
	__int32 started_moving;
	char	u14[ 0x8 ];
	float	lean_yaw;
	char	u15[ 0x8 ];
	float	poses_speed;
	char	u16[ 0x8 ];
	float	ladder_speed;
	char	u17[ 0x8 ];
	float	ladder_yaw;
	char	u18[ 0x8 ];
	float	some_pose;
	char	u19[ 0x14 ];
	float	body_yaw;
	char	u20[ 0x8 ];
	float	body_pitch;
	char	u21[ 0x8 ];
	float	death_yaw;
	char	u22[ 0x8 ];
	float	stand;
	char	u23[ 0x8 ];
	float	jump_fall;
	char	u24[ 0x8 ];
	float	aim_blend_stand_idle;
	char	u25[ 0x8 ];
	float	aim_blend_crouch_idle;
	char	u26[ 0x8 ];
	float	strafe_yaw;
	char	u27[ 0x8 ];
	float	aim_blend_stand_walk;
	char	u28[ 0x8 ];
	float	aim_blend_stand_run;
	char	u29[ 0x8 ];
	float	aim_blend_crouch_walk;
	char	u30[ 0x8 ];
	float	move_blend_walk;
	char	u31[ 0x8 ];
	float	move_blend_run;
	char	u32[ 0x8 ];
	float	move_blend_crouch;
	char	u33[ 0x4 ];
	float	speed;
	__int32 moving_in_any_direction;
	float	acceleration;
	char	u34[ 0x74 ];
	float	crouch_height;
	__int32 is_full_crouched;
	bool	smooth_height_valid;
	char	u35[ 0x3 ];
	float	velocity_subtract_x;
	float	velocity_subtract_y;
	float	velocity_subtract_z;
	float	standing_head_height;
};