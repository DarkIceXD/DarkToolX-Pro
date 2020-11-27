#include "../dependencies/utilities/csgo.hpp"
#include "features/features.hpp"

unsigned long WINAPI initialize(void* instance) {
	csgo::user = License::check();
	if (csgo::user.status == License::Status::VALID) {
		while (!GetModuleHandleA("serverbrowser.dll"))
			Sleep(200);

#ifdef _DEBUG
		console::initialize("csgo-cheat console");
#endif
		csgo::conf = new conf();
		try {
			interfaces::initialize();
			csgo::kits = new kit_parser();
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
		MessageBoxA(nullptr, (std::string("License check failed!\nYour HWID is: ") + std::to_string(csgo::user.hwid) + ".\nTry again later.").c_str(), "DarkToolX", MB_OK | MB_ICONERROR);
	}
	FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
}

unsigned long WINAPI release() {
	hooks::release();

#ifdef _DEBUG
	console::release();
#endif
	delete csgo::conf;
	delete csgo::kits;
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
