#include "License.h"
#include "Utils.h"
#include "SimpleIni.h"
#include <ctime>

constexpr auto name = "darktoolx";

uint32_t License::generate_hwid()
{
	constexpr auto size = 50;
	char hwid[size];
	GetVolumeNameForVolumeMountPointA("C:\\", hwid, size);
	hwid[size - 1] = 0;
	return Util::hash(hwid);
}

License::User License::check()
{
	const auto server = "darkicexd.github.io";
	const auto object = "/darktool/keys.ini";
	User user = {};
	user.hwid = License::generate_hwid();
	CSimpleIniA ini;
	if (Util::hash(server) == 729258906494008491U && Util::hash(object) == 3076906676292336621U)
	{
		const auto data = Util::download(server, object);
		ini.LoadData(data.data(), data.size());
	}
	const auto section = std::to_string(user.hwid);
	const auto permission = ini.GetBoolValue(section.c_str(), name);
	if (!permission)
		return user;
	user.uid = ini.GetLongValue(section.c_str(), "uid");
	const time_t expire_time = std::stoll(ini.GetValue(section.c_str(), "time", "0"));
	const auto time = std::time(nullptr);
	if (expire_time == 0)
	{
		user.status = Status::VALID;
	}
	else if (expire_time > time)
	{
		user.status = Status::VALID;
		user.time_left = expire_time - time;
	}
	return user;
}