#pragma once
#include <vector>

namespace util
{
	constexpr uint64_t hash(const char* str)
	{
		uint64_t hash = 5381;
		for (; *str != '\0'; ++str)
			hash = ((hash << 5) + hash) + (*str);
		return hash;
	}
	std::vector<char> download(const char* server, const char* dir);
}