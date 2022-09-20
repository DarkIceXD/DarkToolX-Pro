#include "user_db.h"
#include <fstream>

void user_db::load(const char* file_name)
{
	std::ifstream stream(file_name, std::ios_base::binary);
	if (stream.good())
	{
		const auto json = nlohmann::json::from_msgpack(stream, true, false);
		load(json);
	}
}

void user_db::load(const std::vector<char>& buffer)
{
	const auto json = nlohmann::json::from_msgpack(buffer, true, false);
	load(json);
}

void user_db::load(const nlohmann::json& json)
{
	if (!json.is_discarded())
	{
		users = json.value<std::vector<user>>("users", {});
	}
}

void user_db::save(const char* file_name) const
{
	const auto bytes = nlohmann::json::to_msgpack(*this);
	std::ofstream stream(file_name, std::ios_base::binary);
	stream.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
}