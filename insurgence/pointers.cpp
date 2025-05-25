#include "pointers.h"

#include "globals.h"
#include "signatures.h"

bool Pointers::Create()
{
	if (!this->PrintGrabInterface<IClientEntityList>("Entity List", "client.dll", "VClientEntityList003", this->EntityList))
		return false;

	if (!this->PrintGrabInterface<IVEngineClient>("Engine Client", "engine.dll", "VEngineClient014", this->Client))
		return false;

	if (!this->PrintGrabInterface<IVModelInfo>("Model Info", "engine.dll", "VModelInfoClient006", this->ModelInfo))
		return false;

	//uintptr_t GetClientStateAddr = Memory::FindSignature("engine.dll", Engine_GetClientState);

	//if (GetClientStateAddr)
	//	this->ClientState = reinterpret_cast<CClientState* (__cdecl*)()>(GetClientStateAddr)(); // TODO: Something wack is happening here

	return true;
}

void Pointers::Destroy()
{
	// TODO: Destructors
}