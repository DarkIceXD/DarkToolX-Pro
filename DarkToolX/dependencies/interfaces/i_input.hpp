#pragma once
#include "../../source-sdk/classes/c_usercmd.hpp"

struct i_input {
	char pad0[ 12 ];
	bool track_ir_available;
	bool mouse_initialized;
	bool mouse_active;
	char pad1[178];
	bool camera_in_third_person;
	bool camera_moving_with_mouse;
	vec3_t camera_offset;
	c_usercmd *get_user_cmd( int slot, int sequence_num ) {
		using fn = c_usercmd * ( __thiscall * )( void *, int, int );
		return ( *( fn ** ) this )[ 8 ]( this, slot, sequence_num );
	}
};
