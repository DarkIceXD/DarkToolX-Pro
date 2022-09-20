#pragma once
#include "json.hpp"
#include <bitset>

struct user_db {
	void load(const char* file_name);
	void load(const std::vector<char>& buffer);
	void load(const nlohmann::json& json);
	void save(const char* file_name) const;
	struct user {
		uint64_t uid, expire, tools;
		std::vector<uint64_t> hwid;
		friend std::ostream& operator<<(std::ostream& ostream, const user& user) {
			ostream
				<< "UID: " << user.uid << '\n'
				<< "Expire: " << user.expire << '\n'
				<< "Tools: " << std::bitset<sizeof(user.tools) * 8>(user.tools) << '\n'
				<< "HWID: ";
			for (const auto hwid : user.hwid)
				ostream << hwid << ", ";
			ostream << '\n';
			return ostream;
		}
		enum TOOL_FLAGS {
			NONE,
			DARKINJECT0R = 1 << 0,
			DARKTOOL = 1 << 1,
			DARKTOOLX = 1 << 2,
			DARKTOOL_WARZONE = 1 << 3,
		};
		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(user, uid, expire, tools, hwid)
	};
	[[nodiscard]] constexpr uint64_t get_highest_uid() {
		uint64_t highest = 0;
		for (const auto& user : users)
			if (user.uid > highest)
				highest = user.uid;
		return highest;
	}
	[[nodiscard]] constexpr std::optional<user> find_user_by_hwid(const uint64_t my_hwid) {
		for (const auto& user : users)
			for (const auto& hwid : user.hwid)
				if (hwid == my_hwid)
					return user;
		return std::nullopt;
	}
	std::vector<user> users;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(user_db, users)
};

constexpr std::array tools = { "DARKINJECT0R", "DARKTOOL", "DARKTOOLX", "DARKTOOL_WARZONE" };