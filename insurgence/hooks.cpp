#include "hooks.h"

#include "globals.h"
#include "minhook/MinHook.h"

// TODO: Move to new file
#include "vector.h"
#include "cusercmd.h"

typedef bool (*fnCreateMove)(void* _this, float SampleTime, CUserCmd* Command);
fnCreateMove oCreateMove;

bool __fastcall hkCreateMove(void* _this, float SampleTime, CUserCmd* Command)
{
	return oCreateMove(_this, SampleTime, Command);
}

//typedef void (*fnCLMove)(float Samples, bool Final);
//fnCLMove oCLMove;
//
//void hkCLMove(float Samples, bool Final)
//{
//	oCLMove(Samples, Final);
//}

bool Hooks::Create()
{
	if (MH_Initialize() != MH_OK)
		return false;

	uintptr_t CreateMoveAddr = Globals->MemoryManager->FindSignature("client.dll", "40 53 48 83 EC ? 83 C9 ? 0F 29 74 24");
	//uintptr_t CL_MoveAddr = Globals->MemoryManager->FindSignature("engine.dll", "48 89 5C 24 ? 57 48 83 EC ? 0F 29 74 24 ? 0F B6 DA");
	//uintptr_t CL_SendMoveAddr = Globals->MemoryManager->FindSignature("engine.dll", "48 89 5C 24 ? 57 B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? E8");

	MH_CreateHook((LPVOID)CreateMoveAddr, (LPVOID)&hkCreateMove, (LPVOID*)&oCreateMove);

	MH_EnableHook(MH_ALL_HOOKS);

	return true;
}

void Hooks::Destroy()
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}