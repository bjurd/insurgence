#include "directx.h"

#include "../features/esp.h"
#include "../globals.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
#include "../kiero.h"
#include <d3d9.h>

typedef long (*fnReset)(LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*);
fnReset oReset;

typedef long (*fnEndScene)(LPDIRECT3DDEVICE9);
fnEndScene oEndScene;

static uintptr_t SteamOverlayStart = 0;
static uintptr_t SteamOverlayEnd = 0;

long __fastcall hkReset(LPDIRECT3DDEVICE9 Device, D3DPRESENT_PARAMETERS* PresentationParameters)
{
    ImGui_ImplDX9_InvalidateDeviceObjects();

    long Result = oReset(Device, PresentationParameters);

    ImGui_ImplDX9_CreateDeviceObjects();

    return Result;
}

long __stdcall hkEndScene(LPDIRECT3DDEVICE9 Device)
{
    uintptr_t ReturnAddr = reinterpret_cast<uintptr_t>(_ReturnAddress());

    if (ReturnAddr >= SteamOverlayStart && ReturnAddr < SteamOverlayEnd)
        return oEndScene(Device); // Don't render in overlay

    static bool SetupUI = false;

    if (!SetupUI)
    {
        SetupUI = true;

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
    }

    ImGui_ImplWin32_NewFrame();
    ImGui_ImplDX9_NewFrame();
    ImGui::NewFrame();

    if (ImGui::Begin("Hello World", nullptr, ImGuiWindowFlags_NoCollapse))
    {
        ImGui::End();
    }

    ImGui::EndFrame();

    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    static ESP* ESPFeature = (ESP*)Globals->FeaturesManager->Get("ESP");

    if (ESPFeature)
        ESPFeature->Render(Device);

    return oEndScene(Device);
}

void DirectX::Create()
{
    if (kiero::init(kiero::RenderType::D3D9) != kiero::Status::Success)
    {
        printf("No init\n");
        return;
    }

    if (kiero::bind(16, (void**)&oReset, hkReset) != kiero::Status::Success)
    {
        printf("No reset\n");
        return;
    }

    if (kiero::bind(42, (void**)&oEndScene, hkEndScene) != kiero::Status::Success)
    {
        printf("No endscene\n");
        return;
    }

    uintptr_t Base = 0;
    size_t Size = 0;

    if (Memory::GetModuleInfo(L"gameoverlayrenderer64.dll", Base, Size))
    {
        SteamOverlayStart = Base;
        SteamOverlayEnd = Base + Size;
    }
}

void DirectX::Destroy()
{
    kiero::unbind(16);
    kiero::unbind(42);

    kiero::shutdown();
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}