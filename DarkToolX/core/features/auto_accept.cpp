#include "features.hpp"

void features::auto_accept()
{
	if (!csgo::conf->misc().auto_accept)
		return;

	static auto fn_accept = reinterpret_cast<bool(__stdcall*)(const char*)>(utilities::pattern_scan("client.dll", "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12"));
	if (fn_accept)
		fn_accept("");
}