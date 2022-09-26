#include "license.h"
#include "utils.h"
#include "xorstr.h"
#include <Windows.h>
#include <ctime>
#include "json_utils.h"

uint64_t license::generate_hwid()
{
	HW_PROFILE_INFOA hwprofinfo;
	if (!GetCurrentHwProfileA(&hwprofinfo))
		return 0;

	return util::hash(hwprofinfo.szHwProfileGuid);
}

license::result license::check(const user_db::user::TOOL_FLAGS flag)
{
	const auto server = xorstr("darkicexd.github.io");
	const auto object = xorstr("/darktool/users");

	result user{};
	user.my_hwid = license::generate_hwid();
	if (!user.my_hwid)
	{
		user.status = status::INVALID_HWID;
		return user;
	}

	if (util::hash(server) != 729258906494008491U || util::hash(object) != 820864865764569909U)
	{
		user.status = status::USER_NOT_FOUND;
		return user;
	}

	const auto db = json_utils::load<user_db>(util::download(server, object), json_utils::type::MSGPACK);
	const auto opt_user = db.find_user_by_hwid(user.my_hwid);
	if (!opt_user)
	{
		user.status = status::USER_NOT_FOUND;
		return user;
	}

	user.user = *opt_user;
	if (!(user.user.tools & flag))
	{
		user.status = status::NO_PERMISSION;
		return user;
	}

	if (user.user.expire != 0 && user.user.expire < std::time(nullptr))
	{
		user.status = status::EXPIRED;
		return user;
	}

	user.status = status::VALID;
	return user;
}

const char* license::status_to_str(const status s)
{
	switch (s)
	{
	case status::INVALID_HWID:
		return "Invalid HWID";
	case status::USER_NOT_FOUND:
		return "User not found";
	case status::NO_PERMISSION:
		return "No permission for this tool";
	case status::EXPIRED:
		return "Expired";
	case status::VALID:
		return "Valid";
	default:
		return "Invalid";
	}
}
