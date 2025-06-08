#include "menu.h"

#include "../binds.h"
#include "../features.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"

#include "aimbot.h"
#include "esp.h"

void Menu::Create()
{
	this->IsSetup = false;
	this->IsOpen = false;

	Binds::Create(VK_INSERT, BindMode::Toggle, std::function([](bool IsPressed, bool WasPressed)
		{
			static Menu* MenuFeature = (Menu*)g_Features->Get("Menu");

			if (MenuFeature)
				MenuFeature->IsOpen = !MenuFeature->IsOpen;
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
		static Aimbot* AimbotFeature = (Aimbot*)g_Features->Get("Aimbot");

		static ESP* ESPFeature = (ESP*)g_Features->Get("ESP");

		if (ESPFeature)
		{
			ImGui::Checkbox("ESP", &ESPFeature->Enabled);
			ImGui::Checkbox("Player Boxes", &ESPFeature->Boxes);
			ImGui::Checkbox("Player Names", &ESPFeature->Names);
		}
	}
	ImGui::End();

	ImGui::EndFrame();

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}
