#include "License.h"
#include "Utils.h"
#include "SimpleIni.h"
#include "str_obfuscator.hpp"
#include <ctime>

uint32_t License::generate_hwid()
{
	constexpr auto size = 50;
	char hwid[size];
	GetVolumeNameForVolumeMountPointA(cryptor::create("C:\\").decrypt(), hwid, size);
	hwid[size - 1] = 0;
	return Util::hash(hwid);
}

License::User License::check()
{
	const auto tool = cryptor::create("darktoolx").decrypt();
	const auto server = cryptor::create("darkicexd.github.io").decrypt();
	const auto object = cryptor::create("/darktool/keys.ini").decrypt();
	User user = {};
	user.hwid = License::generate_hwid();
	CSimpleIniA ini;
	if (Util::hash(server) == 729258906494008491U && Util::hash(object) == 3076906676292336621U)
	{
		const auto data = Util::download(server, object);
		ini.LoadData(data.data(), data.size());
	}
	const auto section = std::to_string(user.hwid);
	if (!ini.GetBoolValue(section.c_str(), tool))
		return user;
	user.uid = ini.GetLongValue(section.c_str(), cryptor::create("uid").decrypt());
	const time_t expire_time = std::stoll(ini.GetValue(section.c_str(), cryptor::create("time").decrypt(), cryptor::create("0").decrypt()));
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
	else
	{
		user.status = Status::EXPIRED;
	}
	return user;
}