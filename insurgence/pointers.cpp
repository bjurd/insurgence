#include "pointers.h"

#include "globals.h"

void Pointers::Create()
{
	this->EntityList = Memory::GetInterface<IClientEntityList*>("client.dll", "VClientEntityList003");

	this->Client = Memory::GetInterface<IVEngineClient*>("engine.dll", "VEngineClient014");

	uintptr_t GetClientStateAddr = Memory::FindSignature("engine.dll", "48 8B 05 ? ? ? ? 48 83 C0");

	if (GetClientStateAddr)
		this->ClientState = reinterpret_cast<CClientState * (__cdecl*)()>(GetClientStateAddr)(); // TODO: Something whack is happening here
}

void Pointers::Destroy()
{

}