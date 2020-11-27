#pragma once
#include <vector>
#include <string>

struct kit_parser
{
	kit_parser();
	struct paint_kit
	{
		int id;
		std::string name;
		bool operator<(const paint_kit& other) const noexcept
		{
			return name < other.name;
		}
	};
	int find(const std::vector<paint_kit>& list, const int id) const noexcept
	{
		for (size_t i = 0; i < list.size(); i++)
		{
			if (list.at(i).id == id)
				return i;
		}
		return 0;
	}
	std::vector<paint_kit> paint_kits;
	std::vector<paint_kit> glove_kits;
	std::vector<paint_kit> sticker_kits;
};