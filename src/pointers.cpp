#include "pointers.h"

#include "memory.h"
#include "signatures.h"
#include "valve/interfaces.h"

void Pointers::PrintAddress(const std::string Name, void* Object)
{
	printf(
		"%s: 0x%016" PRIXPTR "\n",
		Name.c_str(),
		reinterpret_cast<uintptr_t>(Object)
	);
}

bool Pointers::Create()
{
	if (!this->PrintGrabInterface<IClientEntityList>("Entity List", "client.dll", VCLIENTENTITYLIST_INTERFACE_VERSION, this->EntityList))
		return false;

	if (!this->PrintGrabInterface<IVEngineClient>("Engine Client", "engine.dll", VENGINE_CLIENT_INTERFACE_VERSION, this->EngineClient))
		return false;

	if (!this->PrintGrabInterface<CHLClient>("HL Client", "client.dll", CLIENT_DLL_INTERFACE_VERSION, this->Client))
		return false;

	uintptr_t GetPlayerViewAddr = Memory::FindSignature("client.dll", CHLClient_GetPlayerView);
	uintptr_t ViewPtrAddr = Memory::RelativeToReal(GetPlayerViewAddr + 0x6, 3, 7);

	if (GetPlayerViewAddr && ViewPtrAddr)
	{
		this->ViewRender = *reinterpret_cast<CViewRender**>(ViewPtrAddr);
		this->PrintAddress("View Render", this->ViewRender);
	}

	if (!this->PrintGrabInterface<CVRenderView>("Render View", "engine.dll", VENGINE_RENDERVIEW_INTERFACE_VERSION, this->RenderView))
		return false;

	if (!this->PrintGrabInterface<IVModelInfo>("Model Info", "engine.dll", VMODELINFO_CLIENT_INTERFACE_VERSION, this->ModelInfo))
		return false;

	if (!this->PrintGrabInterface<CEngineTrace>("Engine Trace", "engine.dll", ENGINETRACE_CLIENT_INTERFACE_VERSION, this->EngineTrace))
		return false;

	if (!this->PrintGrabInterface<IEngineVGuiInternal>("Engine VGui", "engine.dll", VENGINE_VGUI_INTERFACE_VERSION, this->EngineVGui))
		return false;

	if (!this->PrintGrabInterface<ISurface>("Surface", "vguimatsurface.dll", VGUI_SURFACE_INTERFACE_VERSION, this->Surface))
		return false;

	//uintptr_t GetClientStateAddr = Memory::FindSignature("engine.dll", Engine_GetClientState);

	//if (GetClientStateAddr)
	//	this->ClientState = reinterpret_cast<CClientState* (__cdecl*)()>(GetClientStateAddr)(); // TODO: Something wack is happening here

	return true;
}

void Pointers::Destroy()
{
	this->EntityList = nullptr;
	this->EngineClient = nullptr;
	this->ModelInfo = nullptr;
	this->EngineTrace = nullptr;
	this->EngineVGui = nullptr;
	this->Surface = nullptr;
}
