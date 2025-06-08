#include "pointers.h"

#include "memory.h"
#include "signatures.h"
#include "valve/interfaces.h"

bool Pointers::Create()
{
	if (!this->PrintGrabInterface<IClientEntityList>("Entity List", "client.dll", VCLIENTENTITYLIST_INTERFACE_VERSION, this->EntityList))
		return false;

	if (!this->PrintGrabInterface<IVEngineClient>("Engine Client", "engine.dll", VENGINE_CLIENT_INTERFACE_VERSION, this->Client))
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
	this->Client = nullptr;
	this->ModelInfo = nullptr;
	this->EngineTrace = nullptr;
	this->EngineVGui = nullptr;
	this->Surface = nullptr;
}
