#pragma once
#include "../../source-sdk/math/vector3d.hpp"

#define	PITCH_NORM		100			  // non-pitch shifted
#define PITCH_LOW		95			    // other values are possible - 0-255, where 255 is very high
#define PITCH_HIGH		120

struct sound_info_t;

class i_engine_sound
{
public:
	// Precache a particular sample
	virtual bool precache_sound(const char* pSample, bool bPreload = false, bool bIsUISound = false) = 0;
	virtual bool is_precached(const char* pSample) = 0;
	virtual void prefetch_sound(const char* pSample) = 0;
	virtual bool is_looping_sound(const char* pSample) = 0;

	// Just loads the file header and checks for duration (not hooked up for .mp3's yet)
	// Is accessible to server and client though
	virtual float get_sound_duration(const char* pSample) = 0;

	// Pitch of 100 is no pitch shift.  Pitch > 100 up to 255 is a higher pitch, pitch < 100
	// down to 1 is a lower pitch.   150 to 70 is the realistic range.
	// EmitSound with pitch != 100 should be used sparingly, as it's not quite as
	// fast (the pitchshift mixer is not native coded).

	// NOTE: setting iEntIndex to -1 will cause the sound to be emitted from the local
	// player (client-side only)
	virtual int emit_sound(void* filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample,
		float flVolume, float flAttenuation, int nSeed, int iFlags = 0, int iPitch = PITCH_NORM,
		const vec3_t* pOrigin = NULL, const vec3_t* pDirection = NULL, void* pUtlVecOrigins = NULL, bool bUpdatePositions = true, float soundtime = 0.0f, int speakerentity = -1) = 0;

	virtual int emit_sound(void* filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample,
		float flVolume, int iSoundlevel, int nSeed, int iFlags = 0, int iPitch = PITCH_NORM,
		const vec3_t* pOrigin = NULL, const vec3_t* pDirection = NULL, void* pUtlVecOrigins = NULL, bool bUpdatePositions = true, float soundtime = 0.0f, int speakerentity = -1) = 0;

	virtual void emit_sentence_by_index(void* filter, int iEntIndex, int iChannel, int iSentenceIndex,
		float flVolume, int iSoundlevel, int nSeed, int iFlags = 0, int iPitch = PITCH_NORM,
		const vec3_t* pOrigin = NULL, const vec3_t* pDirection = NULL, void* pUtlVecOrigins = NULL, bool bUpdatePositions = true, float soundtime = 0.0f, int speakerentity = -1) = 0;

	virtual void    stop_sound(int iEntIndex, int iChannel, const char* pSample, unsigned int nSoundEntryHash) = 0;
	virtual void    stop_all_sounds(bool bClearBuffers) = 0;
	virtual void    set_room_type(void* filter, int roomType) = 0;
	virtual void    set_player_dsp(void* filter, int dspType, bool fastReset) = 0;
	virtual int     emit_ambient_sound(const char* pSample, float flVolume, int iPitch = PITCH_NORM, int flags = 0, float soundtime = 0.0f) = 0;
	virtual float   get_dist_gain_from_sound_level(int soundlevel, float dist) = 0;
	virtual int		get_guid_for_last_sound_emitted() = 0;
	virtual bool	is_sound_still_playing(int guid) = 0;
	virtual void	stop_sound_by_guid(int guid, bool bForceSync) = 0;
	virtual void	set_volume_by_guid(int guid, float fvol) = 0;
	virtual void    unk() = 0;
	virtual void	get_active_sounds(void* sndlist) = 0;
	virtual void	precache_sentence_group(const char* pGroupName) = 0;
	virtual void	notify_begin_movie_playback() = 0;
	virtual void	notify_end_movie_playback() = 0;
	virtual bool	get_sound_channel_volume(const char* sound, float& flVolumeLeft, float& flVolumeRight) = 0;
	virtual float	get_elapsed_time_by_guid(int guid) = 0;
};