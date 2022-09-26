#pragma once
#include "../../dependencies/imgui/imgui.h"
#include "../../dependencies/imgui/imgui_impl_win32.h"
#include "../../dependencies/imgui/imgui_impl_dx9.h"
#include "../../dependencies/imgui/imgui_stdlib.h"
#include "../../dependencies/utilities/config/config.h"
#include "../../dependencies/utilities/kit_parser.h"

namespace ImGui {
	void HelpMarker(const char* szDescription);
	void KeyBind(const char* label, int* selection, int* key_bind, bool* did_find);
};

namespace menu {
	void init(HWND window, IDirect3DDevice9* device);
	void render(bool& enabled, conf& cfg);
};