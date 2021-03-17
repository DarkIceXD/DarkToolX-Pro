#include "menu.h"
#include "../../dependencies/utilities/kit_parser.h"

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
	style.Colors[ImGuiCol_Tab] = ImVec4(0.61f, 0.00f, 1.00f, 0.24f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
	style.GrabRounding = style.FrameRounding = 6;
	style.ItemSpacing = ImVec2(8, 6);
}

void menu::render(bool& enabled, conf& conf)
{
	ImGui::GetIO().MouseDrawCursor = enabled;
	if (enabled)
	{
		ImGui::Begin("DarkToolX", &enabled);
		if (ImGui::BeginTabBar("tabs"))
		{
			if (ImGui::BeginTabItem("Aimbot"))
			{
				ImGui::Combo("Mode", &conf.aimbot().mode, "Disabled\0Silent Aim\0Legit Aim\0");
				ImGui::Separator();
				if (conf.aimbot().mode != 0)
				{
					ImGui::Checkbox("Enabled", &conf.aimbot().enabled);
					static auto key_found = true;
					ImGui::KeyBind("Key Bind", &conf.aimbot().key_bind_type, &conf.aimbot().key_bind, &key_found);
					if (conf.aimbot().mode == 2)
						ImGui::SliderFloat("Smoothness", &conf.aimbot().smoothness, 1, 10);
					ImGui::SliderFloat("Fov", &conf.aimbot().fov, 0, 180);
					ImGui::Separator();
					ImGui::Checkbox("Head", &conf.aimbot().head);
					ImGui::Checkbox("Chest", &conf.aimbot().chest);
					ImGui::Checkbox("Stomach", &conf.aimbot().stomach);
					ImGui::Checkbox("Legs", &conf.aimbot().legs);
					ImGui::Checkbox("Arms", &conf.aimbot().arms);
					ImGui::Separator();
					ImGui::Checkbox("Auto Shoot", &conf.aimbot().auto_shoot);
					ImGui::Checkbox("Damage Indicator", &conf.aimbot().dmg_indicator);
					if (conf.aimbot().mode == 1)
					{
						ImGui::Checkbox("Auto Cock Revolver", &conf.aimbot().auto_cock_revolver);
						ImGui::Checkbox("Auto Scope", &conf.aimbot().auto_scope);
					}
					ImGui::Combo("Weapon", &conf.aimbot().weapon_selection, config::weapon_list);
					ImGui::SliderInt("Hitchance", &conf.aimbot().get_selected().hitchance, 1, 100);
					if (conf.aimbot().mode == 1)
					{
						ImGui::SliderInt("Min Damage", &conf.aimbot().get_selected().min_dmg, 1, 100);
						ImGui::SliderInt("Min Damage Override", &conf.aimbot().get_selected().min_dmg_override, 1, 100);
						ImGui::Separator();
						static auto min_dmg_override = true;
						ImGui::KeyBind("Min Damage Override Key Bind", &conf.aimbot().min_dmg_override_key_bind_type, &conf.aimbot().min_dmg_override_key_bind, &min_dmg_override);
						ImGui::Checkbox("Min Damage Override Active", &conf.aimbot().min_dmg_override_active);
					}
				}
				ImGui::Separator();
				ImGui::Checkbox("Backtrack", &conf.aimbot().backtrack);
				ImGui::SliderInt("Backtrack time limit", &conf.aimbot().backtrack_time_limit, 1, 200);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Trigger"))
			{
				ImGui::Checkbox("Enabled", &conf.trigger().enabled);
				static auto key_found = true;
				ImGui::KeyBind("Key Bind", &conf.trigger().key_bind_type, &conf.trigger().key_bind, &key_found);
				ImGui::Combo("Weapon", &conf.aimbot().weapon_selection, config::weapon_list);
				ImGui::SliderInt("Hitchance", &conf.aimbot().get_selected().hitchance, 1, 100);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Visuals"))
			{
				ImGui::Combo("Glow", &conf.visuals().selection, "Enemy\0Team\0C4\0Pistol\0SMG\0Rifle\0Shotgun\0Sniper\0MG\0Utility\0");
				auto& rgb = conf.visuals().glow.at(conf.visuals().selection);
				ImGui::Checkbox("Glow Enabled", &rgb.enabled);
				if (conf.visuals().selection == 0)
					ImGui::Checkbox("Health Based", &conf.visuals().health_based);
				if (conf.visuals().selection == 0 && conf.visuals().health_based)
				{
					ImGui::ColorEdit4("Full HP", &rgb.r, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
					ImGui::ColorEdit4("Low HP", &conf.visuals().glow.back().r, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				}
				else
				{
					ImGui::ColorEdit4("Color", &rgb.r, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				}
				ImGui::Separator();
				ImGui::Checkbox("Enemy Overhead Info", &conf.visuals().enemy_overhead_info);
				ImGui::Separator();
				ImGui::Checkbox("Box ESP", &conf.visuals().box_esp);
				ImGui::Checkbox("Dormant ESP", &conf.visuals().dormant_esp);
				ImGui::ColorEdit4("Box Team Color", &conf.visuals().box_team.r, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::ColorEdit4("Box Enemy Color", &conf.visuals().box_enemy.r, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::Checkbox("Aimbot Spot", &conf.visuals().show_aimbot_spot);
				ImGui::Separator();
				ImGui::Checkbox("Step ESP", &conf.visuals().step_color.enabled);
				ImGui::ColorEdit4("Step Color", &conf.visuals().step_color.r, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SliderFloat("Step Max Radius", &conf.visuals().step_max_radius, 100, 1000);
				ImGui::SliderFloat("Step Lifetime", &conf.visuals().step_lifetime, .5f, 8);
				ImGui::SliderFloat("Step Width", &conf.visuals().step_width, 1, 20);
				ImGui::Separator();
				ImGui::Checkbox("Enemy Bullet Tracers", &conf.visuals().bullet_trace_enemy.enabled);
				ImGui::ColorEdit4("Enemy Bullet Tracers Color", &conf.visuals().bullet_trace_enemy.r, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::Checkbox("Team Bullet Tracers", &conf.visuals().bullet_trace_team.enabled);
				ImGui::Checkbox("Local Player Bullet Tracers", &conf.visuals().bullet_trace_local_player);
				ImGui::ColorEdit4("Team Bullet Tracers Color", &conf.visuals().bullet_trace_team.r, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
				ImGui::SliderFloat("Bullet Tracers Lifetime", &conf.visuals().bullet_trace_lifetime, .5f, 8);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("View"))
			{
				ImGui::Checkbox("Thirdperson", &conf.view().thirdperson);
				static auto key_found = true;
				ImGui::KeyBind("Key Bind", &conf.view().key_bind_type, &conf.view().key_bind, &key_found);
				ImGui::SliderFloat("Range", &conf.view().range, 100, 500);
				ImGui::Checkbox("Spectator Thirdperson", &conf.view().spectator_thirdperson);
				ImGui::Separator();
				ImGui::Checkbox("Crosshair", &conf.view().crosshair);
				ImGui::Checkbox("Show Recoil", &conf.view().show_recoil);
				ImGui::Checkbox("Remove Scope", &conf.view().remove_scope);
				ImGui::Separator();
				ImGui::Combo("Skybox", &conf.view().sky_box, sky_list.data(), sky_list.size());
				ImGui::Checkbox("Make skybox darker", &conf.view().dark_skybox);
				ImGui::Checkbox("Night mode", &conf.view().night_mode);
				ImGui::Checkbox("No Fog", &conf.view().no_fog);
				ImGui::Checkbox("No Flash", &conf.view().no_flash);
				ImGui::SliderFloat("View Fov", &conf.view().view_fov, 0, 60);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Skin Changer"))
			{
				ImGui::Checkbox("Enabled", &conf.skin_changer().enabled);
				ImGui::Checkbox("Auto Save", &conf.skin_changer().auto_save);
				ImGui::Combo("Weapon", &conf.skin_changer().selection, config::weapon_list_all);
				ImGui::Separator();
				auto need_to_update = false;
				if (conf.skin_changer().selection == 0)
				{
					if (ImGui::Combo("Glove Type", &conf.skin_changer().glove_selection, "-\0Bloodhound\0Sport\0Driver\0Hand Wraps\0Moto\0Specialist\0Hydra\0Broken Fang\0"))
						need_to_update = true;
				}
				else if (conf.skin_changer().selection == 1)
				{
					if (ImGui::Combo("Knife Type", &conf.skin_changer().knife_selection, "-\0Bayonet\0Classic\0Flip\0Gut\0Karambit\0M9 Bayonet\0Huntsman\0Falchion\0Bowie\0Butterfly\0Shadow Daggers\0Paracord\0Survival\0Ursus\0Navaja\0Nomad\0Stiletto\0Talon\0Skeleton\0"))
						need_to_update = true;
				}
				const auto& kit_list = conf.skin_changer().selection == 0 ? kit_parser::glove_kits : kit_parser::paint_kits;
				const int current_index = kit_parser::find(kit_list, conf.skin_changer().get_selected().paint_kit);
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
								conf.skin_changer().get_selected().paint_kit = kit.id;
								need_to_update = true;
							}
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
				if (ImGui::SliderFloat("Wear", &conf.skin_changer().get_selected().wear, FLT_MIN, 1, "%.5f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_NoRoundToFormat))
					need_to_update = true;
				if (ImGui::InputInt("Stat Trak", &conf.skin_changer().get_selected().stat_trak))
					need_to_update = true;
				if (ImGui::InputInt("Seed", &conf.skin_changer().get_selected().seed))
					need_to_update = true;
				if (ImGui::Combo("Quality", &conf.skin_changer().get_selected().quality, "Normal\0Genuine\0Vintage\0Unusual (Knife)\0Normal\0Community\0Developer\0Self-Made\0Customized\0Strange\0Completed\0Normal\0Tournament\0"))
					need_to_update = true;
				if (ImGui::InputText("Nametag", &conf.skin_changer().get_selected().nametag))
					need_to_update = true;
				if (ImGui::Checkbox("Count Kills", &conf.skin_changer().get_selected().count_kills))
					need_to_update = true;
				if (need_to_update)
					interfaces::clientstate->full_update();
				ImGui::Checkbox("Animated Skins", &conf.skin_changer().get_selected().animated_skins);
				if (conf.skin_changer().get_selected().animated_skins)
				{
					ImGui::SameLine();
					if (ImGui::Button("+"))
						conf.skin_changer().get_selected().animation.push_back({ conf.skin_changer().get_selected().paint_kit, conf.skin_changer().get_selected().stat_trak, conf.skin_changer().get_selected().seed, conf.skin_changer().get_selected().wear, conf.skin_changer().get_selected().nametag.c_str() });
					ImGui::Checkbox("Reverse Animation", &conf.skin_changer().get_selected().reverse);
					ImGui::SliderInt("Delay (Ticks)", &conf.skin_changer().get_selected().delay_ticks, 0, 10);
					ImGui::Columns(4, "generator", false);
					static float start = FLT_MIN;
					ImGui::SliderFloat("Wear start", &start, FLT_MIN, 1, "%.5f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_NoRoundToFormat);
					ImGui::NextColumn();
					static float end = 1;
					ImGui::SliderFloat("Wear end", &end, FLT_MIN, 1, "%.5f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_NoRoundToFormat);
					ImGui::NextColumn();
					static float step = 0.01f;
					ImGui::SliderFloat("Wear step", &step, 0.01f, 0.1f, "%.5f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_NoRoundToFormat);
					ImGui::NextColumn();
					if (ImGui::Button("Generate Wear Animation"))
					{
						if (start < end)
						{
							for (float wear = start; wear <= end; wear += step)
								conf.skin_changer().get_selected().animation.push_back({ conf.skin_changer().get_selected().paint_kit, conf.skin_changer().get_selected().stat_trak, conf.skin_changer().get_selected().seed, std::clamp(wear, FLT_MIN, 1.f), conf.skin_changer().get_selected().nametag.c_str() });
						}
						else
						{
							for (float wear = start; wear >= end; wear -= step)
								conf.skin_changer().get_selected().animation.push_back({ conf.skin_changer().get_selected().paint_kit, conf.skin_changer().get_selected().stat_trak, conf.skin_changer().get_selected().seed, std::clamp(wear, FLT_MIN, 1.f), conf.skin_changer().get_selected().nametag.c_str() });
						}
					}
					ImGui::NextColumn();
					ImGui::Columns(1);
					ImGui::Columns(6, "animation");
					ImGui::Separator();
					ImGui::Text("Paint Kit");
					ImGui::NextColumn();
					ImGui::Text("Stat Trak");
					ImGui::NextColumn();
					ImGui::Text("Seed");
					ImGui::NextColumn();
					ImGui::Text("Wear");
					ImGui::NextColumn();
					ImGui::Text("Nametag");
					ImGui::NextColumn();
					ImGui::Text("Delete");
					ImGui::NextColumn();
					ImGui::Separator();
					int del = -1;
					for (size_t i = 0; i < conf.skin_changer().get_selected().animation.size(); i++)
					{
						auto& e = conf.skin_changer().get_selected().animation.at(i);
						ImGui::PushID(i);
						const int ci = kit_parser::find(kit_list, e.paint_kit);
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
						ImGui::NextColumn();
						ImGui::InputInt("##stat", &e.stat_trak);
						ImGui::NextColumn();
						ImGui::InputInt("##seed", &e.seed);
						ImGui::NextColumn();
						ImGui::SliderFloat("##wear", &e.wear, FLT_MIN, 1, "%.5f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_NoRoundToFormat);
						ImGui::NextColumn();
						ImGui::InputText("##nametag", &e.nametag);
						ImGui::NextColumn();
						if (ImGui::Button("-"))
							del = i;
						ImGui::NextColumn();
						ImGui::PopID();
					}
					if (del >= 0)
						conf.skin_changer().get_selected().animation.erase(conf.skin_changer().get_selected().animation.begin() + del);
					ImGui::Columns(1);
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Clan Tag Changer"))
			{
				ImGui::Checkbox("Enabled", &conf.clan_tag_changer().enabled);
				if (ImGui::BeginCombo("Clan Tag", conf.clan_tag_changer().get_selected().name.c_str()))
				{
					for (size_t i = 0; i < conf.clan_tag_changer().tags.size(); i++)
					{
						const auto& current = conf.clan_tag_changer().tags.at(i);
						const bool is_selected = (conf.clan_tag_changer().selected == i);
						if (ImGui::Selectable(current.name.c_str(), is_selected))
							conf.clan_tag_changer().selected = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
				ImGui::SameLine();
				if (ImGui::Button("+"))
					conf.clan_tag_changer().tags.push_back({});
				ImGui::SameLine();
				if (ImGui::Button("-") && conf.clan_tag_changer().tags.size() > 1)
					conf.clan_tag_changer().tags.erase(conf.clan_tag_changer().tags.begin() + conf.clan_tag_changer().selected);
				ImGui::InputText("Name", &conf.clan_tag_changer().get_selected().name);
				ImGui::SliderInt("Delay", &conf.clan_tag_changer().get_selected().delay, 1, 5000, "%d", ImGuiSliderFlags_AlwaysClamp);
				if (ImGui::Button("Add Tag"))
					conf.clan_tag_changer().get_selected().tags.push_back({});
				ImGui::Columns(2, "tags");
				ImGui::Separator();
				ImGui::Text("Clan Tag");
				ImGui::NextColumn();
				ImGui::Text("Delete");
				ImGui::NextColumn();
				ImGui::Separator();
				int del = -1;
				for (size_t i = 0; i < conf.clan_tag_changer().get_selected().tags.size(); i++)
				{
					ImGui::PushID(i);
					ImGui::InputText("##tag", &conf.clan_tag_changer().get_selected().tags.at(i));
					ImGui::NextColumn();
					if (ImGui::Button("-"))
						del = i;
					ImGui::NextColumn();
					ImGui::PopID();
				}
				if (del >= 0)
					conf.clan_tag_changer().get_selected().tags.erase(conf.clan_tag_changer().get_selected().tags.begin() + del);
				ImGui::Columns(1);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Chat Bot"))
			{
				ImGui::Checkbox("Enabled", &conf.chat_bot().enabled);
				ImGui::SameLine();
				if (ImGui::Button("+"))
					conf.chat_bot().msgs.push_back({});
				ImGui::Columns(2, "messages");
				ImGui::Separator();
				ImGui::Text("Message");
				ImGui::NextColumn();
				ImGui::Text("Delete");
				ImGui::NextColumn();
				ImGui::Separator();
				int del = -1;
				for (size_t i = 0; i < conf.chat_bot().msgs.size(); i++)
				{
					ImGui::PushID(i);
					ImGui::InputText("##msg", &conf.chat_bot().msgs.at(i).msg);
					ImGui::NextColumn();
					if (ImGui::Button("-"))
						del = i;
					ImGui::NextColumn();
					ImGui::PopID();
				}
				if (del >= 0)
					conf.chat_bot().msgs.erase(conf.chat_bot().msgs.begin() + del);
				ImGui::Columns(1);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Misc"))
			{
				ImGui::Combo("Anti Aim", &conf.misc().anti_aim, "Disabled\0Rage\0Custom\0");
				ImGui::Checkbox("Smart Anti Aim", &conf.misc().smart_anti_aim);
				if (conf.misc().anti_aim == 2)
				{
					ImGui::SliderInt("Pitch", &conf.misc().pitch, -89, 89);
					ImGui::SliderInt("Yaw", &conf.misc().yaw, -180, 180);
				}
				ImGui::Separator();
				ImGui::Combo("Desync", &conf.misc().desync, "Disabled\0Left\0Right\0");
				ImGui::SliderInt("Max Desync Delta", &conf.misc().max_desync_delta, 0, 60);
				ImGui::Checkbox("Fake Lag", &conf.misc().fake_lag);
				ImGui::SliderInt("Fake Lag Ticks", &conf.misc().fake_lag_ticks, 1, 6);
				ImGui::Checkbox("Bunny hop", &conf.misc().bhop);
				ImGui::Checkbox("Remove Duck delay", &conf.misc().no_duck_delay);
				ImGui::Checkbox("Slow walk", &conf.misc().slow_walk);
				ImGui::Checkbox("Auto Stop", &conf.misc().auto_stop);
				ImGui::Checkbox("Hitsound", &conf.misc().hitsound);
				ImGui::Checkbox("Auto Switch", &conf.misc().auto_switch);
				ImGui::Checkbox("Auto Accept", &conf.misc().auto_accept);
				ImGui::Checkbox("Reveal Ranks", &conf.misc().reveal_ranks);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Config"))
			{
				if (ImGui::BeginCombo("Aimbot##1", conf.aimbot().conf_name.c_str()))
				{
					for (size_t i = 0; i < conf._aimbot.size(); i++)
					{
						ImGui::PushID(i);
						const auto& current = conf._aimbot.at(i);
						const bool is_selected = (conf.s_aimbot == i);
						if (ImGui::Selectable(current.conf_name.c_str(), is_selected))
							conf.s_aimbot = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
						ImGui::PopID();
					}
					ImGui::EndCombo();
				}
				ImGui::SameLine();
				if (ImGui::Button("+##0"))
					conf._aimbot.push_back({});
				ImGui::SameLine();
				if (ImGui::Button("-##0") && conf._aimbot.size() > 1)
					conf._aimbot.erase(conf._aimbot.begin() + conf.s_aimbot);
				ImGui::SameLine();
				if (ImGui::Button("Dupe##0"))
					conf._aimbot.push_back(conf.aimbot());
				ImGui::SameLine();
				if (ImGui::Button("Share to Clipboard##0"))
					ImGui::SetClipboardText(conf.export_config(0).c_str());
				ImGui::InputText("Aimbot Config Name", &conf.aimbot().conf_name);
				ImGui::Separator();
				if (ImGui::BeginCombo("Trigger##1", conf.trigger().conf_name.c_str()))
				{
					for (size_t i = 0; i < conf._trigger.size(); i++)
					{
						ImGui::PushID(i);
						const auto& current = conf._trigger.at(i);
						const bool is_selected = (conf.s_trigger == i);
						if (ImGui::Selectable(current.conf_name.c_str(), is_selected))
							conf.s_trigger = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
						ImGui::PopID();
					}
					ImGui::EndCombo();
				}
				ImGui::SameLine();
				if (ImGui::Button("+##1"))
					conf._trigger.push_back({});
				ImGui::SameLine();
				if (ImGui::Button("-##1") && conf._trigger.size() > 1)
					conf._trigger.erase(conf._trigger.begin() + conf.s_trigger);
				ImGui::SameLine();
				if (ImGui::Button("Dupe##1"))
					conf._trigger.push_back(conf.trigger());
				ImGui::SameLine();
				if (ImGui::Button("Share to Clipboard##1"))
					ImGui::SetClipboardText(conf.export_config(1).c_str());
				ImGui::InputText("Trigger Config Name", &conf.trigger().conf_name);
				ImGui::Separator();
				if (ImGui::BeginCombo("Visuals##1", conf.visuals().conf_name.c_str()))
				{
					for (size_t i = 0; i < conf._visuals.size(); i++)
					{
						ImGui::PushID(i);
						const auto& current = conf._visuals.at(i);
						const bool is_selected = (conf.s_visuals == i);
						if (ImGui::Selectable(current.conf_name.c_str(), is_selected))
							conf.s_visuals = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
						ImGui::PopID();
					}
					ImGui::EndCombo();
				}
				ImGui::SameLine();
				if (ImGui::Button("+##2"))
					conf._visuals.push_back({});
				ImGui::SameLine();
				if (ImGui::Button("-##2") && conf._visuals.size() > 1)
					conf._visuals.erase(conf._visuals.begin() + conf.s_visuals);
				ImGui::SameLine();
				if (ImGui::Button("Dupe##2"))
					conf._visuals.push_back(conf.visuals());
				ImGui::SameLine();
				if (ImGui::Button("Share to Clipboard##2"))
					ImGui::SetClipboardText(conf.export_config(2).c_str());
				ImGui::InputText("Visuals Config Name", &conf.visuals().conf_name);
				ImGui::Separator();
				if (ImGui::BeginCombo("View##1", conf.view().conf_name.c_str()))
				{
					for (size_t i = 0; i < conf._view.size(); i++)
					{
						ImGui::PushID(i);
						const auto& current = conf._view.at(i);
						const bool is_selected = (conf.s_view == i);
						if (ImGui::Selectable(current.conf_name.c_str(), is_selected))
							conf.s_view = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
						ImGui::PopID();
					}
					ImGui::EndCombo();
				}
				ImGui::SameLine();
				if (ImGui::Button("+##3"))
					conf._view.push_back({});
				ImGui::SameLine();
				if (ImGui::Button("-##3") && conf._view.size() > 1)
					conf._view.erase(conf._view.begin() + conf.s_view);
				ImGui::SameLine();
				if (ImGui::Button("Dupe##3"))
					conf._view.push_back(conf.view());
				ImGui::SameLine();
				if (ImGui::Button("Share to Clipboard##3"))
					ImGui::SetClipboardText(conf.export_config(3).c_str());
				ImGui::InputText("View Config Name", &conf.view().conf_name);
				ImGui::Separator();
				if (ImGui::BeginCombo("Skin Changer##1", conf.skin_changer().conf_name.c_str()))
				{
					for (size_t i = 0; i < conf._skin_changer.size(); i++)
					{
						ImGui::PushID(i);
						const auto& current = conf._skin_changer.at(i);
						const bool is_selected = (conf.s_skin_changer == i);
						if (ImGui::Selectable(current.conf_name.c_str(), is_selected))
							conf.s_skin_changer = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
						ImGui::PopID();
					}
					ImGui::EndCombo();
				}
				ImGui::SameLine();
				if (ImGui::Button("+##4"))
					conf._skin_changer.push_back({});
				ImGui::SameLine();
				if (ImGui::Button("-##4") && conf._skin_changer.size() > 1)
					conf._skin_changer.erase(conf._skin_changer.begin() + conf.s_skin_changer);
				ImGui::SameLine();
				if (ImGui::Button("Dupe##4"))
					conf._skin_changer.push_back(conf.skin_changer());
				ImGui::SameLine();
				if (ImGui::Button("Share to Clipboard##4"))
					ImGui::SetClipboardText(conf.export_config(4).c_str());
				ImGui::InputText("Skin Changer Config Name", &conf.skin_changer().conf_name);
				ImGui::Separator();
				if (ImGui::BeginCombo("Clan Tag Changer##1", conf.clan_tag_changer().conf_name.c_str()))
				{
					for (size_t i = 0; i < conf._clan_tag_changer.size(); i++)
					{
						ImGui::PushID(i);
						const auto& current = conf._clan_tag_changer.at(i);
						const bool is_selected = (conf.s_clan_tag_changer == i);
						if (ImGui::Selectable(current.conf_name.c_str(), is_selected))
							conf.s_clan_tag_changer = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
						ImGui::PopID();
					}
					ImGui::EndCombo();
				}
				ImGui::SameLine();
				if (ImGui::Button("+##5"))
					conf._clan_tag_changer.push_back({});
				ImGui::SameLine();
				if (ImGui::Button("-##5") && conf._clan_tag_changer.size() > 1)
					conf._clan_tag_changer.erase(conf._clan_tag_changer.begin() + conf.s_clan_tag_changer);
				ImGui::SameLine();
				if (ImGui::Button("Dupe##5"))
					conf._clan_tag_changer.push_back(conf.clan_tag_changer());
				ImGui::SameLine();
				if (ImGui::Button("Share to Clipboard##5"))
					ImGui::SetClipboardText(conf.export_config(5).c_str());
				ImGui::InputText("Clan Tag Changer Config Name", &conf.clan_tag_changer().conf_name);
				ImGui::Separator();
				if (ImGui::BeginCombo("Chat Bot##1", conf.chat_bot().conf_name.c_str()))
				{
					for (size_t i = 0; i < conf._chat_bot.size(); i++)
					{
						ImGui::PushID(i);
						const auto& current = conf._chat_bot.at(i);
						const bool is_selected = (conf.s_chat_bot == i);
						if (ImGui::Selectable(current.conf_name.c_str(), is_selected))
							conf.s_chat_bot = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
						ImGui::PopID();
					}
					ImGui::EndCombo();
				}
				ImGui::SameLine();
				if (ImGui::Button("+##"))
					conf._chat_bot.push_back({});
				ImGui::SameLine();
				if (ImGui::Button("-##6") && conf._chat_bot.size() > 1)
					conf._chat_bot.erase(conf._chat_bot.begin() + conf.s_chat_bot);
				ImGui::SameLine();
				if (ImGui::Button("Dupe##6"))
					conf._chat_bot.push_back(conf.chat_bot());
				ImGui::SameLine();
				if (ImGui::Button("Share to Clipboard##6"))
					ImGui::SetClipboardText(conf.export_config(6).c_str());
				ImGui::InputText("Chat Bot Config Name", &conf.chat_bot().conf_name);
				ImGui::Separator();
				if (ImGui::BeginCombo("Misc##1", conf.misc().conf_name.c_str()))
				{
					for (size_t i = 0; i < conf._misc.size(); i++)
					{
						ImGui::PushID(i);
						const auto& current = conf._misc.at(i);
						const bool is_selected = (conf.s_misc == i);
						if (ImGui::Selectable(current.conf_name.c_str(), is_selected))
							conf.s_misc = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
						ImGui::PopID();
					}
					ImGui::EndCombo();
				}
				ImGui::SameLine();
				if (ImGui::Button("+##7"))
					conf._misc.push_back({});
				ImGui::SameLine();
				if (ImGui::Button("-##7") && conf._misc.size() > 1)
					conf._misc.erase(conf._misc.begin() + conf.s_misc);
				ImGui::SameLine();
				if (ImGui::Button("Dupe##7"))
					conf._misc.push_back(conf.misc());
				ImGui::SameLine();
				if (ImGui::Button("Share to Clipboard##7"))
					ImGui::SetClipboardText(conf.export_config(7).c_str());
				ImGui::InputText("Misc Config Name", &conf.misc().conf_name);
				ImGui::Separator();
				if (ImGui::Button("Save"))
					conf.save();
				ImGui::SameLine();
				if (ImGui::Button("Import Config from Clipboard"))
					conf.import_config(ImGui::GetClipboardText());
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
}