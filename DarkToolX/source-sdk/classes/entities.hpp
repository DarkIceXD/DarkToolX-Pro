#pragma once
#include "../../dependencies/math/math.hpp"
#include "collideable.hpp"
#include "client_class.hpp"
#include "../structs/animstate.hpp"
#include "../../dependencies/utilities/netvars/netvars.hpp"
#include "../structs/models.hpp"
#include "../../dependencies/interfaces/interfaces.hpp"

enum data_update_type_t {
	DATA_UPDATE_CREATED = 0,
	DATA_UPDATE_DATATABLE_CHANGED,
};

enum cs_weapon_type {
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER_RIFLE,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_PLACEHOLDER,
	WEAPONTYPE_GRENADE,
	WEAPONTYPE_UNKNOWN
};

enum client_frame_stage_t {
	FRAME_UNDEFINED = -1,			// (haven't run any frames yet)
	FRAME_START,

	// A network packet is being recieved
	FRAME_NET_UPDATE_START,
	// Data has been received and we're going to start calling PostDataUpdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// Data has been received and we've called PostDataUpdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// We've received all packets, we can now do interpolation, prediction, etc..
	FRAME_NET_UPDATE_END,

	// We're about to start rendering the scene
	FRAME_RENDER_START,
	// We've finished rendering the scene.
	FRAME_RENDER_END
};

enum move_type {
	movetype_none = 0,
	movetype_isometric,
	movetype_walk,
	movetype_step,
	movetype_fly,
	movetype_flygravity,
	movetype_vphysics,
	movetype_push,
	movetype_noclip,
	movetype_ladder,
	movetype_observer,
	movetype_custom,
	movetype_last = movetype_custom,
	movetype_max_bits = 4,
	max_movetype
};

enum entity_flags {
	fl_onground = (1 << 0),
	fl_ducking = (1 << 1),
	fl_animducking = (1 << 2),
	fl_waterjump = (1 << 3),
	fl_ontrain = (1 << 4),
	fl_inrain = (1 << 5),
	fl_frozen = (1 << 6),
	fl_atcontrols = (1 << 7),
	fl_client = (1 << 8),
	fl_fakeclient = (1 << 9),
	fl_inwater = (1 << 10),
	fl_fly = (1 << 11),
	fl_swim = (1 << 12),
	fl_conveyor = (1 << 13),
	fl_npc = (1 << 14),
	fl_godmode = (1 << 15),
	fl_notarget = (1 << 16),
	fl_aimtarget = (1 << 17),
	fl_partialground = (1 << 18),
	fl_staticprop = (1 << 19),
	fl_graphed = (1 << 20),
	fl_grenade = (1 << 21),
	fl_stepmovement = (1 << 22),
	fl_donttouch = (1 << 23),
	fl_basevelocity = (1 << 24),
	fl_worldbrush = (1 << 25),
	fl_object = (1 << 26),
	fl_killme = (1 << 27),
	fl_onfire = (1 << 28),
	fl_dissolving = (1 << 29),
	fl_transragdoll = (1 << 30),
	fl_unblockable_by_player = (1 << 31)
};

enum item_definition_index : short
{
	WEAPON_DEAGLE = 1,
	WEAPON_ELITE = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALILAR = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_MP5_SD = 23,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_KNIFE = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKEGRENADE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCGRENADE = 48,
	WEAPON_C4 = 49,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER = 60,
	WEAPON_USP_SILENCER = 61,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_CSS = 503,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516,
	WEAPON_KNIFE_CORD = 517,
	WEAPON_KNIFE_CANIS = 518,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE = 520,
	WEAPON_KNIFE_OUTDOOR = 521,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER = 523,
	WEAPON_KNIFE_SKELETON = 525,
	GLOVE_BROKEN_FANG = 4725,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034,
	GLOVE_HYDRA = 5035
};

class i_ref_counted {
private:
	volatile long refCount;

public:
	virtual void destructor(char bDelete) = 0;
	virtual bool OnFinalRelease() = 0;

	void unreference() {
		if (InterlockedDecrement(&refCount) == 0 && OnFinalRelease()) {
			destructor(1);
		}
	}
};

class entity_t {
public:
	void* animating() {
		return reinterpret_cast<void*>(uintptr_t(this) + 0x4);
	}
	void* networkable() {
		return reinterpret_cast<void*>(uintptr_t(this) + 0x8);
	}
	collideable_t* collideable() {
		using original_fn = collideable_t * (__thiscall*)(void*);
		return (*(original_fn * *)this)[3](this);
	}
	c_client_class* client_class() {
		using original_fn = c_client_class * (__thiscall*)(void*);
		return (*(original_fn * *)networkable())[2](networkable());
	}

	int index() {
		using original_fn = int(__thiscall*)(void*);
		return (*(original_fn * *)networkable())[10](networkable());
	}
	bool is_player() {
		using original_fn = bool(__thiscall*)(entity_t*);
		return (*(original_fn * *)this)[157](this);
	}
	bool is_weapon() {
		using original_fn = bool(__thiscall*)(entity_t*);
		return (*(original_fn * *)this)[165](this);
	}
	bool setup_bones(matrix_t * out, int max_bones, int mask, float time) {
		using original_fn = bool(__thiscall*)(void*, matrix_t*, int, int, float);
		return (*(original_fn * *)animating())[13](animating(), out, max_bones, mask, time);
	}
	model_t* model() {
		using original_fn = model_t * (__thiscall*)(void*);
		return (*(original_fn * *)animating())[8](animating());
	}
	void update() {
		using original_fn = void(__thiscall*)(entity_t*);
		(*(original_fn * *)this)[218](this);
	}
	int draw_model(int flags, uint8_t alpha) {
		using original_fn = int(__thiscall*)(void*, int, uint8_t);
		return (*(original_fn * *)animating())[9](animating(), flags, alpha);
	}
	void set_angles(vec3_t angles) {
		using original_fn = void(__thiscall*)(void*, const vec3_t&);
		static original_fn set_angles_fn = (original_fn)((DWORD)utilities::pattern_scan("client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"));
		set_angles_fn(this, angles);
	}
	void set_position(vec3_t position) {
		using original_fn = void(__thiscall*)(void*, const vec3_t&);
		static original_fn set_position_fn = (original_fn)((DWORD)utilities::pattern_scan("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8 ? ? ? ? 8B 7D 08"));
		set_position_fn(this, position);
	}

	void set_model_index(int index) {
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn * *)this)[75](this, index);
	}

	void net_pre_data_update(int update_type) {
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn * *)networkable())[6](networkable(), update_type);
	}

	void net_post_data_update(int update_type) {
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)networkable())[7](networkable(), update_type);
	}

	void net_on_data_changed(int update_type) {
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)networkable())[5](networkable(), update_type);
	}
	void net_release() {
		using original_fn = void(__thiscall*)(void*);
		return (*(original_fn * *)networkable())[1](networkable());
	}

	int net_set_destroyed_on_recreate_entities() {
		using original_fn = int(__thiscall*)(void*);
		return (*(original_fn * *)networkable())[13](networkable());
	}

	bool dormant( ) {
		using original_fn = bool( __thiscall* )( void* );
		return ( *static_cast< original_fn** >( networkable( ) ) )[ 9 ]( networkable( ) );
	}
	
	NETVAR("DT_CSPlayer", "m_fFlags", flags, int)
	NETVAR("DT_BaseEntity", "m_hOwnerEntity", owner_handle, unsigned long)
	NETVAR("DT_CSPlayer", "m_flSimulationTime", simulation_time, float)
	NETVAR("DT_BasePlayer", "m_vecOrigin", origin, vec3_t)
	NETVAR("DT_BasePlayer", "m_vecViewOffset[0]", view_offset, vec3_t)
	NETVAR("DT_CSPlayer", "m_iTeamNum", team, int)
	NETVAR("DT_BaseEntity", "m_bSpotted", spotted, bool)
	NETVAR("DT_CSPlayer", "m_nSurvivalTeam", survival_team, int)
	NETVAR("DT_CSPlayer", "m_flHealthShotBoostExpirationTime", health_boost_time, float)
	NETVAR_OFFSET("DT_BaseEntity", "m_bIsAutoaimTarget", 0x4, get_index, int)
	NETVAR_OFFSET("DT_BaseEntity", "m_CollisionGroup", -0x30, rgfl_coordinate_frame, matrix_t)
	bool is_enemy(entity_t* entity)
	{
		if (interfaces::game_types->get_current_game_type() == 6)
		{
			auto danger_zone_team = this->survival_team();
			if (danger_zone_team > -1)
				return danger_zone_team != entity->survival_team();
			return true;
		}
		else
		{
			return this->team() != entity->team();
		}
	}
	int max_health() {
		using original_fn = int(__thiscall*)(entity_t*);
		return (*(original_fn**)this)[122](this);
	}
	vec3_t& abs_origin() {
		using original_fn = vec3_t & (__thiscall*)(void*);
		return (*(original_fn**)this)[10](this);
	}
	vec3_t& abs_angles() {
		using original_fn = vec3_t & (__thiscall*)(void*);
		return (*(original_fn**)this)[11](this);
	}

	utl_vector<matrix_t>& get_cached_bones()
	{
		return *reinterpret_cast<utl_vector<matrix_t>*>(uintptr_t(this) + 0x2910);
	}
};

class ragdoll_t : public entity_t
{
public:
	NETVAR("DT_CSRagdoll", "m_vecForce", force, vec3_t)
	NETVAR("DT_CSRagdoll", "m_vecRagdollVelocity", velocity, vec3_t)
};

class econ_view_item_t {
public:
	NETVAR("DT_ScriptCreatedItem", "m_bInitialized", is_initialized, bool)
	NETVAR("DT_ScriptCreatedItem", "m_iEntityLevel", entity_level, int)
	NETVAR("DT_ScriptCreatedItem", "m_iAccountID", account_id, int)
	NETVAR("DT_ScriptCreatedItem", "m_iItemIDLow", item_id_low, int)
};

class econ_item_t {
public:
	utl_vector<i_ref_counted*>& m_CustomMaterials() {
		return *reinterpret_cast<utl_vector<i_ref_counted*>*>(uintptr_t(this) + 0x14);
	}
	utl_vector<i_ref_counted*>& m_VisualsDataProcessors() {
		return *reinterpret_cast<utl_vector<i_ref_counted*>*>(uintptr_t(this) + 0x230);
	}
};

class base_view_model_t : public entity_t {
public:
	NETVAR("DT_BaseViewModel", "m_nModelIndex", model_index, int)
	NETVAR("DT_BaseViewModel", "m_nViewModelIndex", view_model_index, int)
	NETVAR("DT_BaseViewModel", "m_hWeapon", weapon, int)
	NETVAR("DT_BaseViewModel", "m_hOwner", owner, int)
	NETPROP("DT_BaseViewModel", "m_nSequence", sequence_prop)
};

class weapon_t : public entity_t {
public:
	NETVAR("DT_BaseCombatWeapon", "m_flNextPrimaryAttack", next_primary_attack, float)
	NETVAR("DT_BaseCombatWeapon", "m_flNextSecondaryAttack", next_secondary_attack, float)
	NETVAR("DT_BaseCombatWeapon", "m_iClip1", clip1_count, int)
	NETVAR("DT_BaseCombatWeapon", "m_iClip2", clip2_count, int)
	NETVAR("DT_BaseCombatWeapon", "m_iPrimaryReserveAmmoCount", primary_reserve_ammo_acount, int)
	NETVAR("DT_WeaponCSBase", "m_flRecoilIndex", recoil_index, float)
	NETVAR("DT_WeaponCSBaseGun", "m_zoomLevel", zoom_level, float)
	NETVAR("DT_BaseAttributableItem", "m_iItemDefinitionIndex", item_definition_index, short)
	NETVAR("DT_BaseCombatWeapon", "m_iEntityQuality", entity_quality, int)
	NETVAR("DT_WeaponCSBase", "m_flPostponeFireReadyTime", postpone_fire_ready, float)
	NETVAR("DT_BaseAttributableItem", "m_iItemIDHigh", item_id_high, int)
	NETVAR("DT_BaseAttributableItem", "m_flFallbackWear", wear, float)
	NETVAR("DT_BaseAttributableItem", "m_nFallbackPaintKit", paint_kit, int)
	NETVAR("DT_BaseAttributableItem", "m_nFallbackSeed", seed, int)
	NETVAR("DT_BaseAttributableItem", "m_nFallbackStatTrak", stat_trak, int)
	NETVAR("DT_BaseAttributableItem", "m_OriginalOwnerXuidLow", original_owner_xuid, int)
	NETVAR("DT_BaseAttributableItem", "m_iAccountID", account_id, int)
	NETVAR("DT_BaseAttributableItem", "m_szCustomName", custom_name, char[32])
	NETVAR("DT_BaseCombatWeapon", "m_AttributeManager", m_AttributeManager, econ_view_item_t)
	NETVAR("DT_BaseAttributableItem", "m_Item", m_Item, econ_item_t)
	NETVAR("DT_BaseCSGrenade", "m_bPinPulled", is_pin_pulled, bool)
	NETVAR("DT_BaseCSGrenade", "m_fThrowTime", throw_time, float)

	float inaccuracy() {
		using original_fn = float(__thiscall*)(void*);
		return (*(original_fn * *)this)[482](this);
	}

	float get_spread() {
		using original_fn = float(__thiscall*)(void*);
		return (*(original_fn * *)this)[452](this);
	}

	void update_accuracy_penalty() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn * *)this)[483](this);
	}

	weapon_info_t* get_weapon_data() {
		return interfaces::weapon_system->get_weapon_data(this->item_definition_index());
	}

	utl_vector<i_ref_counted*>& m_CustomMaterials() {
		static auto offset = *reinterpret_cast<uintptr_t*>(utilities::pattern_scan("client.dll", "83 BE ? ? ? ? ? 7F 67") + 2) - 12;
		return *reinterpret_cast<utl_vector<i_ref_counted*>*>(uintptr_t(this) + offset);
	}

	bool& m_bCustomMaterialInitialized() {
		static auto offset = *reinterpret_cast<uintptr_t*>(utilities::pattern_scan("client.dll", "C6 86 ? ? ? ? ? FF 50 04") + 2);
		return *reinterpret_cast<bool*>(uintptr_t(this) + offset);
	}
	static const char* get_weapon_name(const short index) {
		switch (index) {
		default: return "Knife";
		case WEAPON_GLOCK: return "Glock-18";
		case WEAPON_HKP2000: return "P2000";
		case WEAPON_USP_SILENCER: return "USP-S";
		case WEAPON_ELITE: return "Dual Berettas";
		case WEAPON_P250: return "P250";
		case WEAPON_TEC9: return "Tec-9";
		case WEAPON_FIVESEVEN: return "Five-SeveN";
		case WEAPON_CZ75A: return "CZ75-Auto";
		case WEAPON_DEAGLE: return "Desert Eagle";
		case WEAPON_REVOLVER: return "R8 Revolver";
		case WEAPON_MAC10: return "MAC-10";
		case WEAPON_MP9: return "MP9";
		case WEAPON_MP7: return "MP7";
		case WEAPON_MP5_SD: return "MP5-SD";
		case WEAPON_UMP45: return "UMP-45";
		case WEAPON_P90: return "P90";
		case WEAPON_BIZON: return "PP-Bizon";
		case WEAPON_GALILAR: return "Galil AR";
		case WEAPON_FAMAS: return "FAMAS";
		case WEAPON_AK47: return "AK-47";
		case WEAPON_M4A1: return "M4A4";
		case WEAPON_M4A1_SILENCER: return "M4A1-S";
		case WEAPON_SG556: return "SG 553";
		case WEAPON_AUG: return "AUG";
		case WEAPON_SSG08: return "SSG 08";
		case WEAPON_AWP: return "AWP";
		case WEAPON_G3SG1: return "G3SG1";
		case WEAPON_SCAR20: return "SCAR-20";
		case WEAPON_NOVA: return "Nova";
		case WEAPON_XM1014: return "XM1014";
		case WEAPON_SAWEDOFF: return "Sawed-Off";
		case WEAPON_MAG7: return "MAG-7";
		case WEAPON_M249: return "M249";
		case WEAPON_NEGEV: return "Negev";
		case WEAPON_FLASHBANG: return "Flashbang";
		case WEAPON_HEGRENADE: return "HE Grenade";
		case WEAPON_SMOKEGRENADE: return "Smoke Grenade";
		case WEAPON_MOLOTOV: return "Molotov";
		case WEAPON_DECOY: return "Decoy Grenade";
		case WEAPON_INCGRENADE: return "Incendiary";
		case WEAPON_C4: return "C4";
		}
	}
	const char* get_weapon_name() {
		return get_weapon_name(item_definition_index());
	}
};

class player_t : public entity_t {
private:
	template <typename T>
	T& read(uintptr_t offset) {
		return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset);
	}

	template <typename T>
	void write(uintptr_t offset, T data) {
		*reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset) = data;
	}

public:
	NETVAR("DT_BasePlayer", "m_hViewModel[0]", view_model, int)
	NETVAR("DT_CSPlayer", "m_bHasDefuser", has_defuser, bool)
	NETVAR("DT_CSPlayer", "m_bGunGameImmunity", has_gun_game_immunity, bool)
	NETVAR("DT_CSPlayer", "m_iShotsFired", shots_fired, int)
	NETVAR("DT_CSPlayer", "m_angEyeAngles", eye_angles, vec3_t)
	NETVAR("DT_CSPlayer", "m_ArmorValue", armor, int)
	NETVAR("DT_CSPlayer", "m_bHasHelmet", has_helmet, bool)
	NETVAR("DT_CSPlayer", "m_bIsScoped", is_scoped, bool)
	NETVAR("DT_CSPlayer", "m_bIsDefusing", is_defusing, bool)
	NETVAR("DT_CSPlayer", "m_iAccount", money, int)
	NETVAR("DT_CSPlayer", "m_flLowerBodyYawTarget", lower_body_yaw, float)
	NETVAR("DT_CSPlayer", "m_flNextAttack", next_attack, float)
	NETVAR("DT_CSPlayer", "m_flFlashDuration", flash_duration, float)
	NETVAR("DT_CSPlayer", "m_flFlashMaxAlpha", flash_alpha, float)
	NETVAR("DT_CSPlayer", "m_bHasNightVision", has_night_vision, bool)
	NETVAR("DT_CSPlayer", "m_bNightVisionOn", night_vision_on, bool)
	NETVAR("DT_CSPlayer", "m_iHealth", health, int)
	NETVAR("DT_CSPlayer", "m_lifeState", life_state, int)
	NETVAR("DT_CSPlayer", "m_fFlags", flags, int)
	NETVAR("DT_BasePlayer", "m_viewPunchAngle", punch_angle, vec3_t)
	NETVAR("DT_BasePlayer", "m_aimPunchAngle", aim_punch_angle, vec3_t)
	NETVAR("DT_BasePlayer", "m_vecVelocity[0]", velocity, vec3_t)
	NETVAR("DT_BasePlayer", "m_flMaxspeed", max_speed, float)
	NETVAR("DT_BaseEntity", "m_flShadowCastDistance", fov_time, float)
	NETVAR("DT_BasePlayer", "m_hObserverTarget", observer_target, unsigned long)
	NETVAR("DT_BasePlayer", "m_nHitboxSet", hitbox_set, int)
	NETVAR("DT_CSPlayer", "m_flDuckAmount", duck_amount, float)
	NETVAR("DT_CSPlayer", "m_bHasHeavyArmor", has_heavy_armor, bool)
	NETVAR("DT_SmokeGrenadeProjectile", "m_nSmokeEffectTickBegin", smoke_grenade_tick_begin, int)
	NETVAR("DT_CSPlayer", "m_nTickBase", get_tick_base, int)
	NETVAR("DT_BasePlayer", "m_iObserverMode", observer_mode, int)
	NETVAR_OFFSET("DT_CSPlayer", "m_hLightingOrigin", -0x18, is_jiggle_bones_enabled, bool)
	NETVAR("DT_BaseEntity", "m_fEffects", get_effects, int)
	NETVAR_OFFSET("DT_BaseEntity", "m_nRenderMode", 1, move_type, int)
	NETVAR_OFFSET("DT_BaseAnimating", "m_hLightingOrigin", 0x3c, anim_layers, utl_vector<anim_layer>)
	weapon_t* active_weapon() {
		auto active_weapon = read<uintptr_t>(netvar_manager::get_net_var(fnv::hash("DT_CSPlayer"), fnv::hash("m_hActiveWeapon"))) & 0xFFF;
		return reinterpret_cast<weapon_t*>(interfaces::entity_list->get_client_entity(active_weapon));
	}

	UINT* get_wearables() {
		return (UINT*)((uintptr_t)this + (netvar_manager::get_net_var(fnv::hash("DT_CSPlayer"), fnv::hash("m_hMyWearables"))));
	}

	UINT* get_weapons() {
		return (UINT*)((uintptr_t)this + (netvar_manager::get_net_var(fnv::hash("DT_CSPlayer"), fnv::hash("m_hMyWeapons"))));
	}

	vec3_t get_eye_pos() {
		return origin() + view_offset();
	}

	anim_state* get_anim_state() {
		static auto offset = *reinterpret_cast<uintptr_t*>(utilities::pattern_scan("client.dll", "8B 8E ? ? ? ? 85 C9 74 3E") + 2);
		return *reinterpret_cast<anim_state * *>(this + offset);
	}

	vec3_t get_bone_position(int bone) {
		matrix_t bone_matrices[128];
		if (setup_bones(bone_matrices, 128, 256, 0.0f))
			return vec3_t{ bone_matrices[bone][0][3], bone_matrices[bone][1][3], bone_matrices[bone][2][3] };
		else
			return vec3_t{ };
	}

	vec3_t get_hitbox_position(int hitbox_id) {
		matrix_t bone_matrix[MAXSTUDIOBONES];

		if (setup_bones(bone_matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f)) {
			auto studio_model = interfaces::model_info->get_studio_model(model());

			if (studio_model) {
				auto hitbox = studio_model->hitbox_set(0)->hitbox(hitbox_id);

				if (hitbox) {
					const auto min = math::transform_vector(hitbox->mins, bone_matrix[hitbox->bone]);
					const auto max = math::transform_vector(hitbox->maxs, bone_matrix[hitbox->bone]);
					return vec3_t((min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f, (min.z + max.z) * 0.5f);
				}
			}
		}
		return vec3_t{};
	}

	bool is_alive() {
		return this->health() > 0;
	}

	bool is_moving() {
		return this->velocity().length() > 0.1f;
	}

	bool is_in_air() {
		return !( this->flags() & fl_onground );
	}

	bool is_flashed() {
		return this->flash_duration() > 0.0f;
	}

	void update_client_side_animations() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn * *)this)[223](this);
	}

	bool& use_new_animation_state() {
		static auto offset = *reinterpret_cast<uintptr_t*>(utilities::pattern_scan("client.dll", "88 87 ? ? ? ? 75") + 0x2);
		return *reinterpret_cast<bool*>(uintptr_t(this) + offset);
	}

	float* get_pose_parameter() {
		return (float*)(this + netvar_manager::get_net_var(fnv::hash("DT_BaseAnimating"), fnv::hash("m_flPoseParameter")));
	}

	void get_anim_layers(anim_layer* layers) {
		auto& overlay = anim_layers();
		if(overlay.base())
			std::memcpy(layers, overlay.base(), sizeof(anim_layer) * overlay.count());
	}

	void set_anim_layers(anim_layer* layers) {
		auto& overlay = anim_layers();
		if (overlay.base())
			std::memcpy(overlay.base(), layers, sizeof(anim_layer) * overlay.count());
	}

	void get_pose_parameters(float* poses) {
		const auto m_poses = get_pose_parameter();
		if (m_poses)
			std::memcpy(poses, m_poses, sizeof(float) * 24);
	}

	void set_pose_parameters(float* poses) {
		const auto m_poses = get_pose_parameter();
		if (m_poses)
			std::memcpy(m_poses, poses, sizeof(float) * 24);
	}

	float max_desync_angle() {
		const auto anim_state = get_anim_state();
		if (!anim_state)
			return 0.0f;

		auto yaw_modifier = (anim_state->stop_to_full_running_fraction * -0.3f - 0.2f) * std::clamp(anim_state->feet_speed_forwards_or_sideways, 0.0f, 1.0f) + 1.0f;

		if (anim_state->duck_amount > 0.0f)
			yaw_modifier += (anim_state->duck_amount * std::clamp(anim_state->feet_speed_unknown_forwards_or_sideways, 0.0f, 1.0f) * (0.5f - yaw_modifier));

		return anim_state->velocity_subtract_y * yaw_modifier;
	}

	vec3_t recoil() {
		static auto recoil_scale = interfaces::console->get_convar("weapon_recoil_scale");
		return aim_punch_angle() * recoil_scale->get_float();
	}

	bool can_shoot()
	{
		if (next_attack() > interfaces::globals->cur_time)
			return false;

		const auto weapon = active_weapon();
		if (!weapon)
			return false;

		if (weapon->clip1_count() < 1)
			return false;

		const auto weapon_data = weapon->get_weapon_data();
		if (!weapon_data)
			return false;
		
		if (!weapon_data->weapon_full_auto && weapon->next_primary_attack() > interfaces::globals->cur_time)
			return false;

		return true;
	}
};