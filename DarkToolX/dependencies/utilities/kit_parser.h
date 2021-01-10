#pragma once
#include <vector>
#include <string>

namespace kit_parser
{
	void initialize() noexcept;
	struct paint_kit
	{
		int id;
		std::string name;
		bool operator<(const paint_kit& other) const noexcept
		{
			return name < other.name;
		}
	};
	int find(const std::vector<paint_kit>& list, const int id) noexcept;
	inline std::vector<paint_kit> paint_kits;
	inline std::vector<paint_kit> glove_kits;
	inline std::vector<paint_kit> sticker_kits;
};