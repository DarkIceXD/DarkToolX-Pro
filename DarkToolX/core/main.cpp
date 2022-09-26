#include "../dependencies/utilities/csgo.hpp"
#include "features/features.hpp"
#include "../dependencies/utilities/kit_parser.h"
#include "../dependencies/utilities/json_utils.h"

unsigned long WINAPI initialize(void* instance) {
	csgo::user = license::check(user_db::user::DARKTOOLX);
	if (csgo::user.status == license::status::VALID) {
		while (!GetModuleHandleA("serverbrowser.dll"))
			Sleep(200);

#ifdef _DEBUG
		console::initialize("csgo-cheat console");
#endif
		csgo::cfg = json_utils::load<conf>(conf::file_name, json_utils::type::JSON);
		try {
			interfaces::initialize();
			kit_parser::initialize();
			render::initialize();
			hooks::initialize();
		}

		catch (const std::runtime_error& error) {
			MessageBoxA(nullptr, error.what(), "error!", MB_OK | MB_ICONERROR);
			FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
		}

		while (!GetAsyncKeyState(VK_END))
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	else
	{
		MessageBoxA(nullptr, std::format("License check failed! ({})\nYour HWID is: {:x}", license::status_to_str(csgo::user.status), csgo::user.my_hwid).c_str(), "DarkToolX", MB_OK | MB_ICONERROR);
	}
	FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
}

unsigned long WINAPI release() {
	hooks::release();

#ifdef _DEBUG
	console::release();
#endif
	json_utils::save(conf::file_name, json_utils::type::JSON, csgo::cfg);
	return TRUE;
}

std::int32_t WINAPI DllMain(const HMODULE instance [[maybe_unused]], const unsigned long reason, const void* reserved [[maybe_unused]] ) {
	DisableThreadLibraryCalls(instance);

	switch (reason) {
	case DLL_PROCESS_ATTACH: {
		if (auto handle = CreateThread(nullptr, NULL, initialize, instance, NULL, nullptr))
			CloseHandle(handle);

		break;
	}

	case DLL_PROCESS_DETACH: {
		release();
		break;
	}
	}

	return true;
}
