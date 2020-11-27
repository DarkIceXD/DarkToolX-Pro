#pragma once
#include <string>

namespace Util
{
	uint64_t hash(const char* str);
	std::string download_file(const char* url);
	void next(int& current_index, const int size);
	void prev(int& current_index, const int size);
}