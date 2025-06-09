#include "menu.h"

#include "../binds.h"
#include "../features.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
#include "../pointers.h"

#include "aimbot.h"
#include "esp.h"

void Menu::Create()
{
	this->IsSetup = false;
	this->IsOpen = false;

	Binds::Create(VK_INSERT, BindMode::Toggle, std::function([](bool IsPressed, bool WasPressed)
		{
			static Menu* MenuFeature = g_Features->Get<Menu>("Menu");

			if (MenuFeature)
			{
				MenuFeature->IsOpen = !MenuFeature->IsOpen;

				if (MenuFeature->IsOpen)
				{
					g_Pointers->Surface->UnlockCursor();
					g_Pointers->Surface->SetCursor(CursorCode::dc_alwaysvisible_push);
				}
				else
				{
					g_Pointers->Surface->SetCursor(CursorCode::dc_alwaysvisible_pop);
				}
			}
		}));
}

void Menu::Destroy()
{
	this->IsSetup = true;
	this->IsOpen = false;
}

void Menu::Setup(LPDIRECT3DDEVICE9 Device)
{
	if (this->IsSetup)
		return;

	ImGui::CreateContext();

	ImGuiIO& IO = ImGui::GetIO();
	IO.MouseDrawCursor = false;
	IO.WantCaptureMouse = true;
	IO.WantCaptureKeyboard = true;
	IO.IniFilename = NULL;
	IO.LogFilename = NULL;

	D3DDEVICE_CREATION_PARAMETERS CreationParameters;
	Device->GetCreationParameters(&CreationParameters);

	ImGui_ImplWin32_Init(CreationParameters.hFocusWindow);
	ImGui_ImplDX9_Init(Device);

	this->IsSetup = true;
}

void Menu::Render()
{
	if (!this->IsOpen)
		return;

	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX9_NewFrame();
	ImGui::NewFrame();

	if (ImGui::Begin("Assistance Software", nullptr, ImGuiWindowFlags_NoCollapse))
	{
		static Aimbot* AimbotFeature = g_Features->Get<Aimbot>("Aimbot");

		if (AimbotFeature)
		{
			if (ImGui::BeginChild("##MainMenu_Aimbot", ImVec2(), ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY))
			{
				ImGui::SeparatorText("Aimbot");

				ImGui::Checkbox("Enabled", &AimbotFeature->Enabled);

				// TODO: Aim key
				ImGui::SliderFloat("FOV", &AimbotFeature->FOV, 0.f, 180.f, "%.0f");

				ImGui::Checkbox("Target Head", &AimbotFeature->TargetHead); // TODO: Multi-select dropdown
				ImGui::Checkbox("Target Body", &AimbotFeature->TargetTorso);
				ImGui::Checkbox("Target Limbs", &AimbotFeature->TargetLimbs);
			}
			ImGui::EndChild();
		}

		static ESP* ESPFeature = g_Features->Get<ESP>("ESP");

		if (ESPFeature)
		{
			if (ImGui::BeginChild("##MainMenu_Visuals", ImVec2(), ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY))
			{
				ImGui::SeparatorText("Visuals");

				ImGui::Indent(10.f);
				if (ImGui::BeginChild("##MainMenu_Visuals_Player", ImVec2(), ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY))
				{
					ImGui::SeparatorText("Players");

					ImGui::Checkbox("ESP", &ESPFeature->Enabled);
					ImGui::Checkbox("Boxes", &ESPFeature->Boxes);
					ImGui::Checkbox("Names", &ESPFeature->Names);
				}
				ImGui::EndChild();
				ImGui::Unindent(10.f);
			}
			ImGui::EndChild();
		}
	}
	ImGui::End();

	ImGui::EndFrame();

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}
