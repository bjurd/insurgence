#include "directx.h"

#include "../binds.h"
#include "../draw.h"
#include "../features.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
#include "../kiero.h"
#include <d3d9.h>

#include "../features/esp.h"
#include "../features/menu.h"

typedef HRESULT(*fnReset)(LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*);
fnReset oReset;

typedef HRESULT(*fnPresent)(LPDIRECT3DDEVICE9, const RECT*, const RECT*, HWND, const RGNDATA*);
fnPresent oPresent;

HRESULT __stdcall hkReset(LPDIRECT3DDEVICE9 Device, D3DPRESENT_PARAMETERS* PresentationParameters)
{
	static ESP* ESPFeature = g_Features->Get<ESP>("ESP");

	if (ESPFeature)
		ESPFeature->DestroyStateBlocks();

	ImGui_ImplDX9_InvalidateDeviceObjects();

	Draw::Destroy();

	HRESULT Result = oReset(Device, PresentationParameters);

	ImGui_ImplDX9_CreateDeviceObjects();

	return Result;
}

HRESULT __stdcall hkPresent(LPDIRECT3DDEVICE9 Device, const RECT* Source, const RECT* Destination, HWND Window, const RGNDATA* Dirty)
{
	Binds::Process();

	static ESP* ESPFeature = g_Features->Get<ESP>("ESP");

	if (ESPFeature)
		ESPFeature->Render(Device);

	static Menu* MenuFeature = g_Features->Get<Menu>("Menu");

	if (MenuFeature)
	{
		MenuFeature->Setup(Device);
		MenuFeature->Render();
	}

	return oPresent(Device, Source, Destination, Window, Dirty);
}

void DirectX::Create()
{
	// TODO: Failure notifications and maybe retries or something
	// Really all these ::Create methods should be bools to signal failure

	if (kiero::init(kiero::RenderType::D3D9) != kiero::Status::Success)
		return;

	if (kiero::bind(16, (void**)&oReset, hkReset) != kiero::Status::Success)
		return;

	if (kiero::bind(17, (void**)&oPresent, hkPresent) != kiero::Status::Success)
		return;
}

void DirectX::Destroy()
{
	kiero::unbind(16);
	kiero::unbind(17);

	kiero::shutdown();
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
