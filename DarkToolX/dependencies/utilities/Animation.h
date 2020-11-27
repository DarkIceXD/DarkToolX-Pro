#pragma once
#include <vector>
#include <string>

namespace Animation
{
	std::vector<std::string> write_and_delete(std::string str);
	std::vector<std::string> write_and_delete_words(std::string str);
	std::vector<std::string> scroll(std::string str);
	std::vector<std::string> replace_dot(std::string str);
	std::vector<std::string> build(const std::string& s, const int animation);
}