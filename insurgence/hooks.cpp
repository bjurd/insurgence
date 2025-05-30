#include "hooks.h"

#include "globals.h"
#include "minhook/MinHook.h"

#include "hooks/createmove.h"
#include "hooks/directx.h"
#include "hooks/paint.h"
#include "hooks/wndproc.h"

bool Hooks::Create()
{
	if (MH_Initialize() != MH_OK)
		return false;

	this->List = std::vector<Hook*>();
	this->List.push_back(new CreateMove());
	this->List.push_back(new DirectX());
	this->List.push_back(new Paint());
	this->List.push_back(new WndProc());

	for (Hook* Current : this->List)
		Current->Create();

	MH_EnableHook(MH_ALL_HOOKS);

	return true;
}

void Hooks::Destroy()
{
	MH_DisableHook(MH_ALL_HOOKS);

	for (Hook* Current : this->List)
	{
		Current->Destroy();

		delete Current;
	}
	this->List.clear();

	MH_RemoveHook(MH_ALL_HOOKS);

	MH_Uninitialize();
}
