#pragma once

class i_game_types
{
public:
	virtual ~i_game_types() {}
	virtual bool initialize(bool force) = 0;
	virtual bool is_initialized() const = 0;

	virtual bool set_game_type_and_mode(const char* gameType, const char* gameMode) = 0;
	virtual bool get_game_type_and_mode_from_alias(const char* alias, int& gameType, int& gameMode) = 0;

	virtual bool set_game_type_and_mode(int gameType, int gameMode) = 0;

	virtual void set_and_parse_extended_server_info(void* pExtendedServerInfo) = 0;

	virtual bool check_should_set_default_game_mode_and_type(const char* mapName) = 0;

	virtual int get_current_game_type() const = 0;
	virtual int get_current_game_mode() const = 0;

	virtual const char* get_current_map_name() = 0;

	virtual const char* get_current_game_type_name_id() = 0;
	virtual const char* get_current_game_mode_name_id() = 0;
};