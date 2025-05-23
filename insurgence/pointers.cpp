#include "pointers.h"

#include "globals.h"
#include "signatures.h"

void Pointers::Create()
{
	this->EntityList = Memory::GetInterface<IClientEntityList*>("client.dll", "VClientEntityList003");

	this->Client = Memory::GetInterface<IVEngineClient*>("engine.dll", "VEngineClient014");

	uintptr_t GetClientStateAddr = Memory::FindSignature("engine.dll", Engine_GetClientState);

	if (GetClientStateAddr)
		this->ClientState = reinterpret_cast<CClientState * (__cdecl*)()>(GetClientStateAddr)(); // TODO: Something whack is happening here
}

void Pointers::Destroy()
{

}