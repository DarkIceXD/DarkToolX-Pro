#pragma once
#include <string>

namespace License
{
	enum class Status
	{
		INVALID,
		EXPIRED,
		VALID
	};
	struct User {
		uint32_t hwid;
		uint32_t uid;
		Status status;
		time_t time_left;
	};
	uint32_t generate_hwid();
	User check();
}