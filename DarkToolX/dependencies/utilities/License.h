#pragma once
#include "user_db.h"

namespace license
{
	enum class status
	{
		INVALID_HWID,
		USER_NOT_FOUND,
		NO_PERMISSION,
		EXPIRED,
		VALID
	};
	struct result
	{
		user_db::user user;
		uint64_t my_hwid;
		status status;
	};
	uint64_t generate_hwid();
	result check(const user_db::user::TOOL_FLAGS flag);
	const char* status_to_str(const status s);
}