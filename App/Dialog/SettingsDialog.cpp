#include "SettingsDialog.h"
#include "Utils.h"

void SettingsDialog::DrawContent()
{
	if (ImGui::BeginTabBar("MainTabs"))
	{
		if (ImGui::BeginTabItem("Visual"))
		{
			DrawVisualTab();

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::Separator();
	if (ImGui::Button("Apply & Close"))
	{
		ImGui::CloseCurrentPopup();
	}
}

void SettingsDialog::DrawVisualTab()
{
	auto config = app->GetConfig();

	ImGui::Text("These settings will also apply when rendering videos!");
	ImGui::Separator();

	if (ImGui::BeginChild("##scrollArea", ImVec2(0, 400), true, ImGuiWindowFlags_HorizontalScrollbar))
	{
		if (ImGui::BeginTabBar("VisualTabs"))
		{
			if (ImGui::BeginTabItem("Piano"))
			{
				auto colors = config->render.GetBarColor();
				float barColor[3]{ colors.x, colors.y, colors.z };
				if (ImGui::ColorPicker3("Bar Color", barColor))
				{
					config->render.SetBarColor(barColor[0], barColor[1], barColor[2]);
					app->GetRenderer()->SetBarColor(barColor[0], barColor[1], barColor[2]);
				}

				colors = config->render.GetBackground();
				float bgColor[3]{ colors.x, colors.y, colors.z };
				if (ImGui::ColorPicker3("Background Color", bgColor))
				{
					config->render.SetBackground(bgColor[0], bgColor[1], bgColor[2]);
					app->GetRenderer()->SetBackgroundColor(bgColor[0], bgColor[1], bgColor[2]);
				}

				ImGui::BeginDisabled(true);
				ImGui::Button("Pick color palette");
				ImGui::EndDisabled();

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Resource Packs"))
			{
				ResourcePackList* packList = app->GetPackList();
				if (packList != nullptr)
				{
					if (ImGui::Button("Refresh Pack List"))
					{
						packList->RefreshList();
						std::shared_ptr<ResourcePack> activePack = packList->GetActivePack();
						app->GetRenderer()->LoadResourcePack(activePack);
					}

					auto& packs = packList->GetPackList();
					size_t packIdx = 0;
					for (const auto& pack : packs)
					{
						ImGui::BeginGroup();
						ImGui::Text(pack->GetName());
						ImGui::Text("By %s", pack->GetAuthor());
						ImGui::Text(pack->GetDescription());

						bool isActivePack = pack == packList->GetActivePack();
						ImGui::BeginDisabled(isActivePack);
						if (isActivePack) ImGui::Button("Pack in use");
						else
						{
							ImGui::PushID(pack.get());
							if (ImGui::Button("Use pack"))
							{
								packList->SwitchPack(packIdx);
								app->GetRenderer()->LoadResourcePack(pack);
							}
							ImGui::PopID();
						}
						ImGui::EndDisabled();

						ImGui::EndGroup();
						if (packIdx != packs.size() - 1)
							ImGui::Separator();

						packIdx++;
					}
				}
				ImGui::EndTabItem();
			}
		}
		ImGui::EndTabBar();
	}

	ImGui::EndChild();
}