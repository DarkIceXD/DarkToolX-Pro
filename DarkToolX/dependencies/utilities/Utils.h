#pragma once
#include <vector>

namespace Util
{
	uint64_t hash(const char* str);
	std::vector<char> download(const char* server, const char* dir);
	void next(int& current_index, const int size);
	void prev(int& current_index, const int size);
}