#include "menu.h"
#include "../../dependencies/utilities/kit_parser.h"
#include "../../dependencies/utilities/json_utils.h"

void ImGui::HelpMarker(const char* description)
{
	SameLine();
	TextDisabled("(?)");
	if (IsItemHovered())
	{
		BeginTooltip();
		PushTextWrapPos(450.f);
		TextUnformatted(description);
		PopTextWrapPos();
		EndTooltip();
	}
}

void ImGui::KeyBind(const char* label, int* selection, int* key_bind, bool* did_find)
{
	PushID(label);
	Combo(label, selection, "Keybind disabled\0Toggle on key\0Active on hold\0");
	if (!*did_find) {
		PushStyleColor(ImGuiCol_Button, ImVec4(0, 0.6f, 0, 1));
		PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.6f, 0.3f, 1));
	}
	const auto clicked = Button("Rebind");
	if (!*did_find)
		PopStyleColor(2);
	if (clicked)
		*did_find = !*did_find;
	SameLine();
	Text("Bind: %d", *key_bind);
	if (!*did_find)
	{
		if (GetIO().KeysDown[VK_DELETE])
		{
			*key_bind = 0;
			*did_find = true;
		}
		else
		{
			for (int i = 0; i < 512; i++)
			{
				if (GetIO().KeysDown[i])
				{
					GetAsyncKeyState(i);
					*key_bind = i;
					*did_find = true;
					break;
				}
			}
		}
	}
	PopID();
}

void menu::init(HWND window, IDirect3DDevice9* device)
{
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);
	/*ImFontGlyphRangesBuilder builder;
	constexpr ImWchar baseRanges[]{
		0x0100, 0x024F, // Latin Extended-A + Latin Extended-B
		0x0300, 0x03FF, // Combining Diacritical Marks + Greek/Coptic
		0x0600, 0x06FF, // Arabic
		0x0E00, 0x0E7F, // Thai
		0
	};
	builder.AddRanges(baseRanges);
	builder.AddRanges(ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	builder.AddRanges(ImGui::GetIO().Fonts->GetGlyphRangesChineseSimplifiedCommon());
	static ImVector<ImWchar> ranges;
	builder.BuildRanges(&ranges);
	ImFontConfig conf;
	conf.GlyphRanges = ranges.Data;
	ImGui::GetIO().Fonts->AddFontDefault(&conf);
	ImGui::GetIO().Fonts->Build();*/
	auto& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.63f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.67f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.67f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.18f, 0.18f, 0.18f, 0.39f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.61f, 0.00f, 1.00f, 0.24f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.61f, 0.00f, 1.00f, 0.39f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.61f, 0.00f, 1.00f, 0.39f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.61f, 0.00f, 1.00f, 0.24f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.61f, 0.00f, 1.00f, 0.73f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.61f, 0.00f, 1.00f, 0.47f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.61f, 0.00f, 1.00f, 0.75f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.61f, 0.00f, 1.00f, 0.78f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.61f, 0.00f, 1.00f, 0.47f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.61f, 0.00f, 1.00f, 0.47f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.61f, 0.00f, 1.00f, 0.24f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.61f, 0.00f, 1.00f, 0.39f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.61f, 0.00f, 1.00f, 0.39f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.61f, 0.00f, 1.00f, 0.24f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.61f, 0.00f, 1.00f, 0.24f);
	style.GrabRounding = style.FrameRounding = 6;
	style.ItemSpacing = ImVec2(8, 6);
}

void menu::render(bool& enabled, conf& cfg)
{
	ImGui::GetIO().MouseDrawCursor = enabled;
	if (enabled)
	{
		ImGui::Begin("DarkToolX", &enabled);
		ImGui::PushItemWidth(-200);
		if (ImGui::BeginTabBar("tabs"))
		{
			if (ImGui::BeginTabItem("Aimbot"))
			{
				ImGui::Combo("Mode", &cfg.aimbot().mode, "Disabled\0Silent Aim\0Legit Aim\0");
				ImGui::Separator();
				if (cfg.aimbot().mode != 0)
				{
					ImGui::Checkbox("Enabled", &cfg.aimbot().aim.enabled);
					static auto key_found = true;
					ImGui::KeyBind("Key Bind", &cfg.aimbot().aim.type, &cfg.aimbot().aim.key_bind, &key_found);
					if (cfg.aimbot().mode == 2)
						ImGui::SliderFloat("Smoothness", &cfg.aimbot().smoothness, 1, 10);
					ImGui::SliderFloat("Fov", &cfg.aimbot().fov, 0, 180);
					ImGui::Separator();
					if (ImGui::BeginTable("hitbox", 3, ImGuiTableFlags_SizingStretchSame))
					{
						ImGui::TableSetupColumn("Hitbox");
						ImGui::TableSetupColumn("Normal");
						ImGui::TableSetupColumn("Override");
						ImGui::TableHeadersRow();
						ImGui::TableNextRow();
						ImGui::PushID(0);
						ImGui::TableNextColumn();
						ImGui::TextUnformatted("Head");
						ImGui::TableNextColumn();
						ImGui::Checkbox("##normal", &cfg.aimbot().hitbox.head);
						ImGui::TableNextColumn();
						ImGui::Checkbox("##override", &cfg.aimbot().hitbox_override.head);
						ImGui::PopID();
						ImGui::PushID(1);
						ImGui::TableNextColumn();
						ImGui::TextUnformatted("Chest");
						ImGui::TableNextColumn();
						ImGui::Checkbox("##normal", &cfg.aimbot().hitbox.chest);
						ImGui::TableNextColumn();
						ImGui::Checkbox("##override", &cfg.aimbot().hitbox_override.chest);
						ImGui::PopID();
						ImGui::PushID(2);
						ImGui::TableNextColumn();
						ImGui::TextUnformatted("Stomach");
						ImGui::TableNextColumn();
						ImGui::Checkbox("##normal", &cfg.aimbot().hitbox.stomach);
						ImGui::TableNextColumn();
						ImGui::Checkbox("##override", &cfg.aimbot().hitbox_override.stomach);
						ImGui::PopID();
						ImGui::PushID(3);
						ImGui::TableNextColumn();
						ImGui::TextUnformatted("Legs");
						ImGui::TableNextColumn();
						ImGui::Checkbox("##normal", &cfg.aimbot().hitbox.legs);
						ImGui::TableNextColumn();
						ImGui::Checkbox("##override", &cfg.aimbot().hitbox_override.legs);
						ImGui::PopID();
						ImGui::PushID(4);
						ImGui::TableNextColumn();
						ImGui::TextUnformatted("Arms");
						ImGui::TableNextColumn();
						ImGui::Checkbox("##normal", &cfg.aimbot().hitbox.arms);
						ImGui::TableNextColumn();
						ImGui::Checkbox("##override", &cfg.aimbot().hitbox_override.arms);
						ImGui::PopID();
						ImGui::EndTable();
					}
					static auto hitbox_override = true;
					ImGui::KeyBind("Hitbox Override Key Bind", &cfg.aimbot().hitbox_override_bind.type, &cfg.aimbot().hitbox_override_bind.key_bind, &hitbox_override);
					ImGui::Checkbox("Hitbox Override Active", &cfg.aimbot().hitbox_override_bind.enabled);
					ImGui::Separator();
					ImGui::Checkbox("Auto Shoot", &cfg.aimbot().auto_shoot);
					ImGui::Checkbox("Auto Wall", &cfg.aimbot().auto_wall);
					ImGui::Checkbox("Auto Duck Peek", &cfg.aimbot().auto_duck_peek);
					ImGui::Checkbox("Auto Cock Revolver", &cfg.aimbot().auto_cock_revolver);
					ImGui::Checkbox("Auto Scope", &cfg.aimbot().auto_scope);
					ImGui::Checkbox("Damage Indicator", &cfg.aimbot().dmg_indicator);
					ImGui::Combo("Weapon", &cfg.aimbot().weapon_selection, config::weapon_list);
					ImGui::SliderInt("Hitchance", &cfg.aimbot().get_selected().hitchance, 1, 100);
					if (cfg.aimbot().mode == 1)
					{
						ImGui::SliderInt("Min Damage", &cfg.aimbot().get_selected().min_dmg, 1, 100);
						ImGui::SliderInt("Min Damage Override", &cfg.aimbot().get_selected().min_dmg_override, 1, 100);
						ImGui::Separator();
						static auto min_dmg_override = true;
						ImGui::KeyBind("Min Damage Override Key Bind", &cfg.aimbot().min_dmg_override.type, &cfg.aimbot().min_dmg_override.key_bind, &min_dmg_override);
						ImGui::Checkbox("Min Damage Override Active", &cfg.aimbot().min_dmg_override.enabled);
					}
				}
				ImGui::Separator();
				ImGui::Checkbox("Backtrack", &cfg.aimbot().backtrack);
				ImGui::SliderInt("Backtrack time limit", &cfg.aimbot().backtrack_time_limit, 1, 200);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Trigger"))
			{
				ImGui::Checkbox("Enabled", &cfg.trigger().bind.enabled);
				static auto key_found = true;
				ImGui::KeyBind("Key Bind", &cfg.trigger().bind.type, &cfg.trigger().bind.key_bind, &key_found);
				ImGui::Checkbox("Auto Wall", &cfg.aimbot().auto_wall);
				ImGui::Combo("Weapon", &cfg.aimbot().weapon_selection, config::weapon_list);
				ImGui::SliderInt("Hitchance", &cfg.aimbot().get_selected().hitchance, 1, 100);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Visuals"))
			{
				ImGui::Combo("Glow", &cfg.visuals().selection, "Enemy\0Team\0C4\0Pistol\0SMG\0Rifle\0Shotgun\0Sniper\0MG\0Utility\0");
				auto& rgb = cfg.visuals().glow.at(cfg.visuals().selection);
				ImGui::Checkbox("Glow Enabled", &rgb.enabled);
				if (cfg.visuals().selection == 0)
					ImGui::Checkbox("Health Based", &cfg.visuals().health_based);
				if (cfg.visuals().selection == 0 && cfg.visuals().health_based)
				{
					ImGui::ColorEdit4("Full HP", &rgb.r, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
					ImGui::ColorEdit4("Low HP", &cfg.visuals().glow.back().r, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				}
				else
				{
					ImGui::ColorEdit4("Color", &rgb.r, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				}
				ImGui::Separator();
				ImGui::Checkbox("ESP", &cfg.visuals().esp);
				ImGui::Checkbox("Dormant ESP", &cfg.visuals().dormant_esp);
				ImGui::ColorEdit4("Box Team Color", &cfg.visuals().box_team.r, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::ColorEdit4("Box Enemy Color", &cfg.visuals().box_enemy.r, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::ColorEdit4("Skeleton Team Color", &cfg.visuals().skeleton_team.r, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::ColorEdit4("Skeleton Enemy Color", &cfg.visuals().skeleton_enemy.r, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::Checkbox("Aimbot Spot", &cfg.visuals().aimbot_spot);
				ImGui::Separator();
				ImGui::Checkbox("Step ESP", &cfg.visuals().step_color.enabled);
				ImGui::ColorEdit4("Step Color", &cfg.visuals().step_color.r, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SliderFloat("Step Max Radius", &cfg.visuals().step_max_radius, 100, 1000);
				ImGui::SliderFloat("Step Lifetime", &cfg.visuals().step_lifetime, .5f, 8);
				ImGui::SliderFloat("Step Width", &cfg.visuals().step_width, 1, 20);
				ImGui::Separator();
				ImGui::Checkbox("Enemy Bullet Tracers", &cfg.visuals().bullet_trace_enemy.enabled);
				ImGui::ColorEdit4("Enemy Bullet Tracers Color", &cfg.visuals().bullet_trace_enemy.r, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::Checkbox("Team Bullet Tracers", &cfg.visuals().bullet_trace_team.enabled);
				ImGui::Checkbox("Local Player Bullet Tracers", &cfg.visuals().bullet_trace_local_player);
				ImGui::ColorEdit4("Team Bullet Tracers Color", &cfg.visuals().bullet_trace_team.r, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SliderFloat("Bullet Tracers Lifetime", &cfg.visuals().bullet_trace_lifetime, .5f, 8);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("View"))
			{
				ImGui::Checkbox("Thirdperson", &cfg.view().thirdperson.enabled);
				static auto key_found = true;
				ImGui::KeyBind("Key Bind", &cfg.view().thirdperson.type, &cfg.view().thirdperson.key_bind, &key_found);
				ImGui::SliderFloat("Range", &cfg.view().range, 100, 500);
				ImGui::Checkbox("Spectator Thirdperson", &cfg.view().spectator_thirdperson);
				ImGui::Separator();
				ImGui::Checkbox("Crosshair", &cfg.view().crosshair);
				ImGui::Checkbox("Show Recoil", &cfg.view().show_recoil);
				ImGui::Checkbox("Remove Scope", &cfg.view().remove_scope);
				ImGui::Separator();
				ImGui::Combo("Skybox", &cfg.view().sky_box, sky_list.data(), sky_list.size());
				ImGui::Checkbox("Make skybox darker", &cfg.view().dark_skybox);
				ImGui::Checkbox("Night mode", &cfg.view().night_mode);
				ImGui::Checkbox("ASUS Walls", &cfg.view().asus_walls);
				ImGui::SliderFloat("ASUS Walls Alpha", &cfg.view().asus_walls_alpha, 0, 1);
				ImGui::Checkbox("No Fog", &cfg.view().no_fog);
				ImGui::Checkbox("No Flash", &cfg.view().no_flash);
				ImGui::SliderFloat("View Fov", &cfg.view().view_fov, 0, 60);
				ImGui::Combo("Modify Smoke", &cfg.view().modify_smoke, "-\0Wireframe\0Remove Smoke\0");
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Skin Changer"))
			{
				ImGui::Checkbox("Enabled", &cfg.skin_changer().enabled);
				ImGui::Checkbox("Auto Save", &cfg.skin_changer().auto_save);
				ImGui::Combo("Weapon", &cfg.skin_changer().selection, config::weapon_list_all);
				ImGui::Separator();
				auto need_to_update = false;
				if (cfg.skin_changer().selection == 0)
				{
					if (ImGui::Combo("Glove Type", &cfg.skin_changer().glove_selection, "-\0Bloodhound\0Sport\0Driver\0Hand Wraps\0Moto\0Specialist\0Hydra\0Broken Fang\0"))
						need_to_update = true;
				}
				else if (cfg.skin_changer().selection == 1)
				{
					if (ImGui::Combo("Knife Type", &cfg.skin_changer().knife_selection, "-\0Bayonet\0Classic\0Flip\0Gut\0Karambit\0M9 Bayonet\0Huntsman\0Falchion\0Bowie\0Butterfly\0Shadow Daggers\0Paracord\0Survival\0Ursus\0Navaja\0Nomad\0Stiletto\0Talon\0Skeleton\0"))
						need_to_update = true;
				}
				const auto& kit_list = cfg.skin_changer().selection == 0 ? kit_parser::glove_kits : kit_parser::paint_kits;
				const int current_index = kit_parser::find(kit_list, cfg.skin_changer().get_selected().paint_kit);
				static ImGuiTextFilter filter;
				filter.Draw("Search Paint Kit");
				if (ImGui::BeginCombo("Paint Kit", kit_list.at(current_index).name.c_str()))
				{
					for (size_t i = 0; i < kit_list.size(); i++)
					{
						const auto& kit = kit_list.at(i);
						if (filter.PassFilter(kit.name.c_str()))
						{
							const bool is_selected = (current_index == i);
							if (ImGui::Selectable(kit.name.c_str(), is_selected))
							{
								cfg.skin_changer().get_selected().paint_kit = kit.id;
								need_to_update = true;
							}
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
				if (ImGui::SliderFloat("Wear", &cfg.skin_changer().get_selected().wear, FLT_MIN, 1, "%.5f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_NoRoundToFormat))
					need_to_update = true;
				if (ImGui::InputInt("Stat Trak", &cfg.skin_changer().get_selected().stat_trak))
					need_to_update = true;
				if (ImGui::InputInt("Seed", &cfg.skin_changer().get_selected().seed))
					need_to_update = true;
				if (ImGui::Combo("Quality", &cfg.skin_changer().get_selected().quality, "Normal\0Genuine\0Vintage\0Unusual (Knife)\0Normal\0Community\0Developer\0Self-Made\0Customized\0Strange\0Completed\0Normal\0Tournament\0"))
					need_to_update = true;
				if (ImGui::InputText("Nametag", &cfg.skin_changer().get_selected().nametag))
					need_to_update = true;
				if (ImGui::Checkbox("Count Kills", &cfg.skin_changer().get_selected().count_kills))
					need_to_update = true;
				ImGui::Separator();
				ImGui::Combo("Sticker Slot", &cfg.skin_changer().get_selected().selected_sticker, "Slot 1\0Slot 2\0Slot 3\0Slot 4\0Slot 5\0");
				const int sticker_index = kit_parser::find(kit_parser::sticker_kits, cfg.skin_changer().get_selected().get_selected_sticker().paint_kit);
				static ImGuiTextFilter sticker_filter;
				sticker_filter.Draw("Search Sticker");
				if (ImGui::BeginCombo("Sticker", kit_parser::sticker_kits.at(sticker_index).name.c_str()))
				{
					for (size_t i = 0; i < kit_parser::sticker_kits.size(); i++)
					{
						const auto& kit = kit_parser::sticker_kits.at(i);
						if (sticker_filter.PassFilter(kit.name.c_str()))
						{
							const bool is_selected = (sticker_index == i);
							if (ImGui::Selectable(kit.name.c_str(), is_selected))
							{
								cfg.skin_changer().get_selected().get_selected_sticker().paint_kit = kit.id;
								need_to_update = true;
							}
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
				if (ImGui::SliderFloat("Sticker Wear", &cfg.skin_changer().get_selected().get_selected_sticker().wear, FLT_MIN, 1, "%.5f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_NoRoundToFormat))
					need_to_update = true;
				if (ImGui::SliderFloat("Scale", &cfg.skin_changer().get_selected().get_selected_sticker().scale, 0.1f, 5))
					need_to_update = true;
				if (ImGui::SliderFloat("Rotation", &cfg.skin_changer().get_selected().get_selected_sticker().rotation, 0, 360))
					need_to_update = true;
				if (need_to_update)
					interfaces::clientstate->full_update();
				ImGui::Separator();
				ImGui::Checkbox("Animated Skins", &cfg.skin_changer().get_selected().animated_skins);
				if (cfg.skin_changer().get_selected().animated_skins)
				{
					ImGui::SameLine();
					if (ImGui::Button("+"))
						cfg.skin_changer().get_selected().animation.push_back({ cfg.skin_changer().get_selected().paint_kit, cfg.skin_changer().get_selected().stat_trak, cfg.skin_changer().get_selected().seed, cfg.skin_changer().get_selected().wear, cfg.skin_changer().get_selected().nametag.c_str() });
					ImGui::Checkbox("Reverse Animation", &cfg.skin_changer().get_selected().reverse);
					ImGui::SliderInt("Delay (Ticks)", &cfg.skin_changer().get_selected().delay_ticks, 0, 10);
					if (ImGui::BeginTable("gen", 4, ImGuiTableFlags_SizingStretchSame))
					{
						ImGui::TableSetupColumn("Start Wear");
						ImGui::TableSetupColumn("End Wear");
						ImGui::TableSetupColumn("Wear Step");
						ImGui::TableHeadersRow();
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						static float start = FLT_MIN;
						ImGui::SliderFloat("##start", &start, FLT_MIN, 1, "%.5f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_NoRoundToFormat);
						ImGui::TableNextColumn();
						static float end = 1;
						ImGui::SliderFloat("##end", &end, FLT_MIN, 1, "%.5f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_NoRoundToFormat);
						ImGui::TableNextColumn();
						static float step = 0.05f;
						ImGui::SliderFloat("##step", &step, 0.05f, 0.1f, "%.5f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_NoRoundToFormat);
						ImGui::TableNextColumn();
						if (ImGui::Button("Generate Wear Animation"))
						{
							if (start < end)
							{
								for (float wear = start; wear <= end; wear += step)
									cfg.skin_changer().get_selected().animation.push_back({ cfg.skin_changer().get_selected().paint_kit, cfg.skin_changer().get_selected().stat_trak, cfg.skin_changer().get_selected().seed, std::clamp(wear, FLT_MIN, 1.f), cfg.skin_changer().get_selected().nametag.c_str() });
							}
							else
							{
								for (float wear = start; wear >= end; wear -= step)
									cfg.skin_changer().get_selected().animation.push_back({ cfg.skin_changer().get_selected().paint_kit, cfg.skin_changer().get_selected().stat_trak, cfg.skin_changer().get_selected().seed, std::clamp(wear, FLT_MIN, 1.f), cfg.skin_changer().get_selected().nametag.c_str() });
							}
						}
						ImGui::EndTable();
					}
					if (ImGui::BeginTable("anim", 6, ImGuiTableFlags_SizingStretchSame))
					{
						ImGui::TableSetupColumn("Paint Kit");
						ImGui::TableSetupColumn("Stat Trak");
						ImGui::TableSetupColumn("Seed");
						ImGui::TableSetupColumn("Wear");
						ImGui::TableSetupColumn("Nametag");
						ImGui::TableSetupColumn("Delete");
						ImGui::TableHeadersRow();
						int del = -1;
						for (size_t i = 0; i < cfg.skin_changer().get_selected().animation.size(); i++)
						{
							ImGui::TableNextRow();
							ImGui::PushID(i);
							auto& e = cfg.skin_changer().get_selected().animation.at(i);
							const int ci = kit_parser::find(kit_list, e.paint_kit);
							ImGui::TableNextColumn();
							if (ImGui::BeginCombo("##paintkit", kit_list.at(ci).name.c_str()))
							{
								for (size_t i = 0; i < kit_list.size(); i++)
								{
									const auto& kit = kit_list.at(i);
									const bool is_selected = (ci == i);
									if (ImGui::Selectable(kit.name.c_str(), is_selected))
									{
										e.paint_kit = kit.id;
									}
									if (is_selected)
										ImGui::SetItemDefaultFocus();
								}
								ImGui::EndCombo();
							}
							ImGui::TableNextColumn();
							ImGui::InputInt("##stat", &e.stat_trak);
							ImGui::TableNextColumn();
							ImGui::InputInt("##seed", &e.seed);
							ImGui::TableNextColumn();
							ImGui::SliderFloat("##wear", &e.wear, FLT_MIN, 1, "%.5f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_NoRoundToFormat);
							ImGui::TableNextColumn();
							ImGui::InputText("##nametag", &e.nametag);
							ImGui::TableNextColumn();
							if (ImGui::Button("-"))
								del = i;
							ImGui::PopID();
						}
						ImGui::EndTable();
					}
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Clan Tag Changer"))
			{
				ImGui::Checkbox("Enabled", &cfg.clan_tag_changer().enabled);
				if (ImGui::BeginCombo("Clan Tag", cfg.clan_tag_changer().get_selected().name.c_str()))
				{
					for (size_t i = 0; i < cfg.clan_tag_changer().tags.size(); i++)
					{
						const auto& current = cfg.clan_tag_changer().tags.at(i);
						const bool is_selected = (cfg.clan_tag_changer().selected == i);
						if (ImGui::Selectable(current.name.c_str(), is_selected))
							cfg.clan_tag_changer().selected = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
				ImGui::SameLine();
				if (ImGui::Button("+"))
					cfg.clan_tag_changer().tags.push_back({});
				ImGui::SameLine();
				if (ImGui::Button("-") && cfg.clan_tag_changer().tags.size() > 1)
					cfg.clan_tag_changer().tags.erase(cfg.clan_tag_changer().tags.begin() + cfg.clan_tag_changer().selected);
				ImGui::InputText("Name", &cfg.clan_tag_changer().get_selected().name);
				ImGui::SliderInt("Delay", &cfg.clan_tag_changer().get_selected().delay, 1, 5000, "%d", ImGuiSliderFlags_AlwaysClamp);
				if (ImGui::Button("Add Tag"))
					cfg.clan_tag_changer().get_selected().tags.push_back({});
				if (ImGui::BeginTable("tags", 2, ImGuiTableFlags_SizingStretchSame))
				{
					ImGui::TableSetupColumn("Clan Tag");
					ImGui::TableSetupColumn("Delete");
					ImGui::TableHeadersRow();
					int del = -1;
					for (size_t i = 0; i < cfg.clan_tag_changer().get_selected().tags.size(); i++)
					{
						ImGui::TableNextRow();
						ImGui::PushID(i);
						ImGui::TableNextColumn();
						ImGui::InputText("##tag", &cfg.clan_tag_changer().get_selected().tags.at(i));
						ImGui::TableNextColumn();
						if (ImGui::Button("-"))
							del = i;
						ImGui::PopID();
					}
					if (del >= 0)
						cfg.clan_tag_changer().get_selected().tags.erase(cfg.clan_tag_changer().get_selected().tags.begin() + del);
					ImGui::EndTable();
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Chat Bot"))
			{
				ImGui::Checkbox("Enabled", &cfg.chat_bot().enabled);
				ImGui::SameLine();
				if (ImGui::Button("+"))
					cfg.chat_bot().msgs.push_back({});
				if (ImGui::BeginTable("tags", 2, ImGuiTableFlags_SizingStretchSame))
				{
					ImGui::TableSetupColumn("Message");
					ImGui::TableSetupColumn("Delete");
					ImGui::TableHeadersRow();
					int del = -1;
					for (size_t i = 0; i < cfg.chat_bot().msgs.size(); i++)
					{
						ImGui::TableNextRow();
						ImGui::PushID(i);
						ImGui::TableNextColumn();
						ImGui::InputText("##msg", &cfg.chat_bot().msgs.at(i).msg);
						ImGui::TableNextColumn();
						if (ImGui::Button("-"))
							del = i;
						ImGui::PopID();
					}
					if (del >= 0)
						cfg.chat_bot().msgs.erase(cfg.chat_bot().msgs.begin() + del);
					ImGui::EndTable();
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Misc"))
			{
				ImGui::Combo("Anti Aim", &cfg.misc().anti_aim, "Disabled\0Rage\0Custom\0");
				ImGui::Checkbox("Smart Anti Aim", &cfg.misc().smart_anti_aim);
				if (cfg.misc().anti_aim == 2)
				{
					ImGui::SliderInt("Pitch", &cfg.misc().pitch, -89, 89);
					ImGui::SliderInt("Yaw", &cfg.misc().yaw, -180, 180);
				}
				ImGui::Separator();
				ImGui::Combo("Desync", &cfg.misc().desync, "Disabled\0Left\0Right\0Jitter\0");
				ImGui::SliderInt("Max Desync Delta", &cfg.misc().max_desync_delta, 0, 60);
				ImGui::Checkbox("Fake Lag", &cfg.misc().fake_lag);
				ImGui::SliderInt("Fake Lag Ticks", &cfg.misc().fake_lag_ticks, 1, 6);
				ImGui::Checkbox("Bunny hop", &cfg.misc().bhop);
				ImGui::Checkbox("Auto Strafe", &cfg.misc().auto_strafe);
				ImGui::Checkbox("Remove Duck delay", &cfg.misc().no_duck_delay);
				ImGui::Checkbox("Slow walk", &cfg.misc().slow_walk);
				ImGui::Checkbox("Auto Stop", &cfg.misc().auto_stop);
				ImGui::Checkbox("Fake Duck", &cfg.misc().fake_duck.enabled);
				static bool fake_duck = true;
				ImGui::KeyBind("Fake Duck Key Bind", &cfg.misc().fake_duck.type, &cfg.misc().fake_duck.key_bind, &fake_duck);
				ImGui::Checkbox("Walk Bot", &cfg.misc().walk_bot);
				ImGui::Checkbox("Hitsound", &cfg.misc().hitsound);
				ImGui::Checkbox("Quick Switch", &cfg.misc().quick_switch);
				ImGui::Checkbox("Auto Accept", &cfg.misc().auto_accept);
				ImGui::Checkbox("Reveal Ranks", &cfg.misc().reveal_ranks);
				ImGui::Checkbox("Resolver", &cfg.misc().resolver);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Logs"))
			{
				ImGui::PushID(0);
				ImGui::Checkbox("Deaths", &cfg.logs().deaths.enabled);
				ImGui::Checkbox("Local Player", &cfg.logs().deaths.local_player);
				ImGui::SameLine();
				ImGui::Checkbox("Team", &cfg.logs().deaths.team);
				ImGui::SameLine();
				ImGui::Checkbox("Enemy", &cfg.logs().deaths.enemy);
				ImGui::PopID();
				ImGui::Separator();
				ImGui::PushID(1);
				ImGui::Checkbox("Damage", &cfg.logs().damage.enabled);
				ImGui::Checkbox("Local Player", &cfg.logs().damage.local_player);
				ImGui::SameLine();
				ImGui::Checkbox("Team", &cfg.logs().damage.team);
				ImGui::SameLine();
				ImGui::Checkbox("Enemy", &cfg.logs().damage.enemy);
				ImGui::PopID();
				ImGui::Separator();
				ImGui::PushID(2);
				ImGui::Checkbox("Pickup", &cfg.logs().pickup.enabled);
				ImGui::Checkbox("Local Player", &cfg.logs().pickup.local_player);
				ImGui::SameLine();
				ImGui::Checkbox("Team", &cfg.logs().pickup.team);
				ImGui::SameLine();
				ImGui::Checkbox("Enemy", &cfg.logs().pickup.enemy);
				ImGui::PopID();
				ImGui::Separator();
				ImGui::Checkbox("Votes", &cfg.logs().votes);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Config"))
			{
				if (ImGui::BeginTable("configs", 7, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_RowBg))
				{
					ImGui::TableSetupColumn("Config", ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableSetupColumn("Selected");
					ImGui::TableSetupColumn("Name");
					ImGui::TableSetupColumn("Add", ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableSetupColumn("Delete", ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableSetupColumn("Duplicate", ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableSetupColumn("Share", ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableHeadersRow();
					ImGui::TableNextRow();
					ImGui::PushID(0);
					{
						ImGui::TableNextColumn();
						ImGui::TextUnformatted("Aimbot");
						ImGui::TableNextColumn();
						if (ImGui::BeginCombo("##select", cfg.aimbot().conf_name.c_str()))
						{
							for (size_t i = 0; i < cfg._aimbot.size(); i++)
							{
								ImGui::PushID(i);
								const auto& current = cfg._aimbot.at(i);
								const bool is_selected = (cfg.s_aimbot == i);
								if (ImGui::Selectable(current.conf_name.c_str(), is_selected))
									cfg.s_aimbot = i;

								if (is_selected)
									ImGui::SetItemDefaultFocus();
								ImGui::PopID();
							}
							ImGui::EndCombo();
						}
						ImGui::TableNextColumn();
						ImGui::InputText("##name", &cfg.aimbot().conf_name);
						ImGui::TableNextColumn();
						if (ImGui::Button("+"))
							cfg._aimbot.push_back({});
						ImGui::TableNextColumn();
						if (ImGui::Button("-") && cfg._aimbot.size() > 1)
							cfg._aimbot.erase(cfg._aimbot.begin() + cfg.s_aimbot);
						ImGui::TableNextColumn();
						if (ImGui::Button("Dupe"))
							cfg._aimbot.push_back(cfg.aimbot());
						ImGui::TableNextColumn();
						if (ImGui::Button("Share to Clipboard"))
							ImGui::SetClipboardText(cfg.export_config(0).c_str());
					}
					ImGui::PopID();
					ImGui::TableNextRow();
					ImGui::PushID(1);
					{
						ImGui::TableNextColumn();
						ImGui::TextUnformatted("Trigger");
						ImGui::TableNextColumn();
						if (ImGui::BeginCombo("##select", cfg.trigger().conf_name.c_str()))
						{
							for (size_t i = 0; i < cfg._trigger.size(); i++)
							{
								ImGui::PushID(i);
								const auto& current = cfg._trigger.at(i);
								const bool is_selected = (cfg.s_trigger == i);
								if (ImGui::Selectable(current.conf_name.c_str(), is_selected))
									cfg.s_trigger = i;

								if (is_selected)
									ImGui::SetItemDefaultFocus();
								ImGui::PopID();
							}
							ImGui::EndCombo();
						}
						ImGui::TableNextColumn();
						ImGui::InputText("##name", &cfg.trigger().conf_name);
						ImGui::TableNextColumn();
						if (ImGui::Button("+"))
							cfg._trigger.push_back({});
						ImGui::TableNextColumn();
						if (ImGui::Button("-") && cfg._trigger.size() > 1)
							cfg._trigger.erase(cfg._trigger.begin() + cfg.s_trigger);
						ImGui::TableNextColumn();
						if (ImGui::Button("Dupe"))
							cfg._trigger.push_back(cfg.trigger());
						ImGui::TableNextColumn();
						if (ImGui::Button("Share to Clipboard"))
							ImGui::SetClipboardText(cfg.export_config(1).c_str());
					}
					ImGui::PopID();
					ImGui::TableNextRow();
					ImGui::PushID(2);
					{
						ImGui::TableNextColumn();
						ImGui::TextUnformatted("Visuals");
						ImGui::TableNextColumn();
						if (ImGui::BeginCombo("##select", cfg.visuals().conf_name.c_str()))
						{
							for (size_t i = 0; i < cfg._visuals.size(); i++)
							{
								ImGui::PushID(i);
								const auto& current = cfg._visuals.at(i);
								const bool is_selected = (cfg.s_visuals == i);
								if (ImGui::Selectable(current.conf_name.c_str(), is_selected))
									cfg.s_visuals = i;

								if (is_selected)
									ImGui::SetItemDefaultFocus();
								ImGui::PopID();
							}
							ImGui::EndCombo();
						}
						ImGui::TableNextColumn();
						ImGui::InputText("##name", &cfg.visuals().conf_name);
						ImGui::TableNextColumn();
						if (ImGui::Button("+"))
							cfg._visuals.push_back({});
						ImGui::TableNextColumn();
						if (ImGui::Button("-") && cfg._visuals.size() > 1)
							cfg._visuals.erase(cfg._visuals.begin() + cfg.s_visuals);
						ImGui::TableNextColumn();
						if (ImGui::Button("Dupe"))
							cfg._visuals.push_back(cfg.visuals());
						ImGui::TableNextColumn();
						if (ImGui::Button("Share to Clipboard"))
							ImGui::SetClipboardText(cfg.export_config(2).c_str());
					}
					ImGui::PopID();
					ImGui::TableNextRow();
					ImGui::PushID(3);
					{
						ImGui::TableNextColumn();
						ImGui::TextUnformatted("View");
						ImGui::TableNextColumn();
						if (ImGui::BeginCombo("##select", cfg.view().conf_name.c_str()))
						{
							for (size_t i = 0; i < cfg._view.size(); i++)
							{
								ImGui::PushID(i);
								const auto& current = cfg._view.at(i);
								const bool is_selected = (cfg.s_view == i);
								if (ImGui::Selectable(current.conf_name.c_str(), is_selected))
									cfg.s_view = i;

								if (is_selected)
									ImGui::SetItemDefaultFocus();
								ImGui::PopID();
							}
							ImGui::EndCombo();
						}
						ImGui::TableNextColumn();
						ImGui::InputText("##name", &cfg.view().conf_name);
						ImGui::TableNextColumn();
						if (ImGui::Button("+"))
							cfg._view.push_back({});
						ImGui::TableNextColumn();
						if (ImGui::Button("-") && cfg._view.size() > 1)
							cfg._view.erase(cfg._view.begin() + cfg.s_view);
						ImGui::TableNextColumn();
						if (ImGui::Button("Dupe"))
							cfg._view.push_back(cfg.view());
						ImGui::TableNextColumn();
						if (ImGui::Button("Share to Clipboard"))
							ImGui::SetClipboardText(cfg.export_config(3).c_str());
					}
					ImGui::PopID();
					ImGui::TableNextRow();
					ImGui::PushID(4);
					{
						ImGui::TableNextColumn();
						ImGui::TextUnformatted("Skin Changer");
						ImGui::TableNextColumn();
						if (ImGui::BeginCombo("##select", cfg.skin_changer().conf_name.c_str()))
						{
							for (size_t i = 0; i < cfg._skin_changer.size(); i++)
							{
								ImGui::PushID(i);
								const auto& current = cfg._skin_changer.at(i);
								const bool is_selected = (cfg.s_skin_changer == i);
								if (ImGui::Selectable(current.conf_name.c_str(), is_selected))
									cfg.s_skin_changer = i;

								if (is_selected)
									ImGui::SetItemDefaultFocus();
								ImGui::PopID();
							}
							ImGui::EndCombo();
						}
						ImGui::TableNextColumn();
						ImGui::InputText("##name", &cfg.skin_changer().conf_name);
						ImGui::TableNextColumn();
						if (ImGui::Button("+"))
							cfg._skin_changer.push_back({});
						ImGui::TableNextColumn();
						if (ImGui::Button("-") && cfg._skin_changer.size() > 1)
							cfg._skin_changer.erase(cfg._skin_changer.begin() + cfg.s_skin_changer);
						ImGui::TableNextColumn();
						if (ImGui::Button("Dupe"))
							cfg._skin_changer.push_back(cfg.skin_changer());
						ImGui::TableNextColumn();
						if (ImGui::Button("Share to Clipboard"))
							ImGui::SetClipboardText(cfg.export_config(4).c_str());
					}
					ImGui::PopID();
					ImGui::TableNextRow();
					ImGui::PushID(5);
					{
						ImGui::TableNextColumn();
						ImGui::TextUnformatted("Clan Tag Changer");
						ImGui::TableNextColumn();
						if (ImGui::BeginCombo("##select", cfg.clan_tag_changer().conf_name.c_str()))
						{
							for (size_t i = 0; i < cfg._clan_tag_changer.size(); i++)
							{
								ImGui::PushID(i);
								const auto& current = cfg._clan_tag_changer.at(i);
								const bool is_selected = (cfg.s_clan_tag_changer == i);
								if (ImGui::Selectable(current.conf_name.c_str(), is_selected))
									cfg.s_clan_tag_changer = i;

								if (is_selected)
									ImGui::SetItemDefaultFocus();
								ImGui::PopID();
							}
							ImGui::EndCombo();
						}
						ImGui::TableNextColumn();
						ImGui::InputText("##name", &cfg.clan_tag_changer().conf_name);
						ImGui::TableNextColumn();
						if (ImGui::Button("+"))
							cfg._clan_tag_changer.push_back({});
						ImGui::TableNextColumn();
						if (ImGui::Button("-") && cfg._clan_tag_changer.size() > 1)
							cfg._clan_tag_changer.erase(cfg._clan_tag_changer.begin() + cfg.s_clan_tag_changer);
						ImGui::TableNextColumn();
						if (ImGui::Button("Dupe"))
							cfg._clan_tag_changer.push_back(cfg.clan_tag_changer());
						ImGui::TableNextColumn();
						if (ImGui::Button("Share to Clipboard"))
							ImGui::SetClipboardText(cfg.export_config(5).c_str());
					}
					ImGui::PopID();
					ImGui::TableNextRow();
					ImGui::PushID(6);
					{
						ImGui::TableNextColumn();
						ImGui::TextUnformatted("Chat Bot");
						ImGui::TableNextColumn();
						if (ImGui::BeginCombo("##select", cfg.chat_bot().conf_name.c_str()))
						{
							for (size_t i = 0; i < cfg._chat_bot.size(); i++)
							{
								ImGui::PushID(i);
								const auto& current = cfg._chat_bot.at(i);
								const bool is_selected = (cfg.s_chat_bot == i);
								if (ImGui::Selectable(current.conf_name.c_str(), is_selected))
									cfg.s_chat_bot = i;

								if (is_selected)
									ImGui::SetItemDefaultFocus();
								ImGui::PopID();
							}
							ImGui::EndCombo();
						}
						ImGui::TableNextColumn();
						ImGui::InputText("##name", &cfg.chat_bot().conf_name);
						ImGui::TableNextColumn();
						if (ImGui::Button("+"))
							cfg._chat_bot.push_back({});
						ImGui::TableNextColumn();
						if (ImGui::Button("-") && cfg._chat_bot.size() > 1)
							cfg._chat_bot.erase(cfg._chat_bot.begin() + cfg.s_chat_bot);
						ImGui::TableNextColumn();
						if (ImGui::Button("Dupe"))
							cfg._chat_bot.push_back(cfg.chat_bot());
						ImGui::TableNextColumn();
						if (ImGui::Button("Share to Clipboard"))
							ImGui::SetClipboardText(cfg.export_config(6).c_str());
					}
					ImGui::PopID();
					ImGui::TableNextRow();
					ImGui::PushID(7);
					{
						ImGui::TableNextColumn();
						ImGui::TextUnformatted("Misc");
						ImGui::TableNextColumn();
						if (ImGui::BeginCombo("##select", cfg.misc().conf_name.c_str()))
						{
							for (size_t i = 0; i < cfg._misc.size(); i++)
							{
								ImGui::PushID(i);
								const auto& current = cfg._misc.at(i);
								const bool is_selected = (cfg.s_misc == i);
								if (ImGui::Selectable(current.conf_name.c_str(), is_selected))
									cfg.s_misc = i;

								if (is_selected)
									ImGui::SetItemDefaultFocus();
								ImGui::PopID();
							}
							ImGui::EndCombo();
						}
						ImGui::TableNextColumn();
						ImGui::InputText("##name", &cfg.misc().conf_name);
						ImGui::TableNextColumn();
						if (ImGui::Button("+"))
							cfg._misc.push_back({});
						ImGui::TableNextColumn();
						if (ImGui::Button("-") && cfg._misc.size() > 1)
							cfg._misc.erase(cfg._misc.begin() + cfg.s_misc);
						ImGui::TableNextColumn();
						if (ImGui::Button("Dupe"))
							cfg._misc.push_back(cfg.misc());
						ImGui::TableNextColumn();
						if (ImGui::Button("Share to Clipboard"))
							ImGui::SetClipboardText(cfg.export_config(7).c_str());
					}
					ImGui::PopID();
					ImGui::TableNextRow();
					ImGui::PushID(8);
					{
						ImGui::TableNextColumn();
						ImGui::TextUnformatted("Logs");
						ImGui::TableNextColumn();
						if (ImGui::BeginCombo("##select", cfg.logs().conf_name.c_str()))
						{
							for (size_t i = 0; i < cfg._logs.size(); i++)
							{
								ImGui::PushID(i);
								const auto& current = cfg._logs.at(i);
								const bool is_selected = (cfg.s_logs == i);
								if (ImGui::Selectable(current.conf_name.c_str(), is_selected))
									cfg.s_logs = i;

								if (is_selected)
									ImGui::SetItemDefaultFocus();
								ImGui::PopID();
							}
							ImGui::EndCombo();
						}
						ImGui::TableNextColumn();
						ImGui::InputText("##name", &cfg.logs().conf_name);
						ImGui::TableNextColumn();
						if (ImGui::Button("+"))
							cfg._logs.push_back({});
						ImGui::TableNextColumn();
						if (ImGui::Button("-") && cfg._logs.size() > 1)
							cfg._logs.erase(cfg._logs.begin() + cfg.s_logs);
						ImGui::TableNextColumn();
						if (ImGui::Button("Dupe"))
							cfg._logs.push_back(cfg.logs());
						ImGui::TableNextColumn();
						if (ImGui::Button("Share to Clipboard"))
							ImGui::SetClipboardText(cfg.export_config(8).c_str());
					}
					ImGui::PopID();
					ImGui::EndTable();
				}
				ImGui::Separator();
				if (ImGui::Button("Save"))
					json_utils::save(conf::file_name, json_utils::type::JSON, cfg);
				ImGui::SameLine();
				if (ImGui::Button("Import Config from Clipboard"))
					cfg.import_config(ImGui::GetClipboardText());
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::PopItemWidth();
		ImGui::End();
	}
}