#include "pointers.h"

#include "globals.h"
#include "signatures.h"

void Pointers::Create()
{
	this->EntityList = Memory::GetInterface<IClientEntityList*>("client.dll", "VClientEntityList003");

	this->Client = Memory::GetInterface<IVEngineClient*>("engine.dll", "VEngineClient014");

	uintptr_t GetClientStateAddr = Memory::FindSignature("engine.dll", Engine_GetClientState);

	if (GetClientStateAddr)
		this->ClientState = reinterpret_cast<CClientState * (__cdecl*)()>(GetClientStateAddr)(); // TODO: Something wack is happening here

	this->ModelInfo = Memory::GetInterface<IVModelInfo*>("engine.dll", "VModelInfoClient006");

	std::cout << "Entity List: 0x" << std::uppercase << std::hex << this->EntityList << std::endl;
	std::cout << "Client: 0x" << std::uppercase << std::hex << this->Client << std::endl;
	std::cout << "Client State: 0x" << std::uppercase << std::hex << this->ClientState << std::endl;
	std::cout << "Model Info: 0x" << std::uppercase << std::hex << this->ModelInfo << std::endl;
}

void Pointers::Destroy()
{
	// TODO: Destructors
}