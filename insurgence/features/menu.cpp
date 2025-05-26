#include "menu.h"

#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"

void Menu::Create()
{
    this->IsSetup = false;
	this->IsOpen = false;
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
    if (GetAsyncKeyState(VK_INSERT) & 0x0001) // TODO: Use ImGui button system
        this->IsOpen = !this->IsOpen;

	if (!this->IsOpen)
		return;

    ImGui_ImplWin32_NewFrame();
    ImGui_ImplDX9_NewFrame();
    ImGui::NewFrame();

    if (ImGui::Begin("Hello World", nullptr, ImGuiWindowFlags_NoCollapse))
    {

    }
    ImGui::End();

    ImGui::EndFrame();

    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}
