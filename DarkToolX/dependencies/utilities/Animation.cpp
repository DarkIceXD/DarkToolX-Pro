#include "Animation.h"
constexpr auto CLAN_TAG_MAX_SIZE = 15;

std::vector<std::string> Animation::write_and_delete(std::string str)
{
	const auto size = str.size();
	if (size > CLAN_TAG_MAX_SIZE)
		str.erase(CLAN_TAG_MAX_SIZE);
	std::vector<std::string> tags;
	for (auto i = 1; i <= size; i++)
		tags.push_back(str.substr(0, i));

	for (int i = tags.size() - 2; i >= 0; i--)
		tags.push_back(tags[i]);
	tags.push_back("");

	return tags;
}

std::vector<std::string> Animation::write_and_delete_words(std::string str)
{
	constexpr auto delim = ' ';
	std::vector<std::string> tags;
	std::size_t current, previous = 0;
	do {
		current = str.find(delim, previous);
		const auto& temp_tags = write_and_delete(str.substr(previous, current - previous));
		tags.insert(tags.end(), temp_tags.begin(), temp_tags.end());
		previous = current + 1;
	} while (current != std::string::npos);

	return tags;
}

std::vector<std::string> Animation::scroll(std::string str)
{
	const auto size = str.size();
	do {
		str.append(' ' + str.substr(0, CLAN_TAG_MAX_SIZE - 1));
	} while (str.size() - size < CLAN_TAG_MAX_SIZE);
	std::vector<std::string> tags;
	for (auto i = 0; i < size + 1; i++)
		tags.push_back(str.substr(i, CLAN_TAG_MAX_SIZE));

	return tags;
}

std::vector<std::string> Animation::replace_dot(std::string str)
{
	const auto size = str.size();
	if (size > CLAN_TAG_MAX_SIZE)
		str.erase(CLAN_TAG_MAX_SIZE);
	std::vector<std::string> tags;
	std::string punkt;
	punkt.assign(str.size(), '.');
	tags.push_back(punkt);
	for (auto i = 0; i < size; i++)
	{
		const auto c = str[i];
		for (int j = punkt.size() - 1; j >= i; j--)
		{
			std::string punkt2(punkt);
			punkt2[j] = c;
			tags.push_back(punkt2);
		}
		punkt[i] = c;
	}
	for (int i = tags.size() - 2; i > 0; i--)
		tags.push_back(tags[i]);

	return tags;
}

std::vector<std::string> Animation::build(const std::string& s, const int animation)
{
	switch (animation)
	{
	case 2:
		return write_and_delete_words(s);
	case 3:
		return scroll(s);
	case 4:
		return replace_dot(s);
	default:
		return write_and_delete(s);
	}
}