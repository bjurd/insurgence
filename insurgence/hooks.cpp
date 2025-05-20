#include "hooks.h"

#include "globals.h"
#include "minhook/MinHook.h"
#include "createmove.h"

bool Hooks::Create()
{
	if (MH_Initialize() != MH_OK)
		return false;

	this->List = std::vector<Hook*>();
	this->List.push_back(new CreateMove());

	for (Hook* Current : this->List)
		Current->Create();

	MH_EnableHook(MH_ALL_HOOKS);

	return true;
}

void Hooks::Destroy()
{
	for (Hook* Current : this->List)
		Current->Destroy();

	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);

	MH_Uninitialize();
}