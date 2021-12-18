#pragma once

class weapon_info_t {
public:
	char pad0[20];
	int max_clip;
	char pad1[112];
	const char* name;
	char pad2[60];
	int type;
	char pad3[4];
	int price;
	char pad4[8];
	float cycletime;
	char pad5[12];
	bool full_auto;
	char pad6[3];
	int damage;
	float headshot_multiplier;
	float armor_ratio;
	int bullets;
	float penetration;
	char pad7[8];
	float range;
	float range_modifier;
	char pad8[16];
	bool silencer;
	char pad9[15];
	float max_speed;
	float max_speed_alt;
	char pad10[100];
	float recoil_magnitude;
	float recoil_magnitude_alt;
	char pad11[16];
	float recovery_time_stand;
};
