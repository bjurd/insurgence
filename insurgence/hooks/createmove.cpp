#include "createmove.h"

#include "../features.h"
#include "../memory.h"
#include "../minhook/MinHook.h"
#include "../signatures.h"
#include "../valve/cusercmd.h"

#include "../features/aimbot.h"

typedef bool (*fnCreateMove)(void* _this, float SampleTime, CUserCmd* Command);
fnCreateMove oCreateMove;

bool __fastcall hkCreateMove(void* _this, float SampleTime, CUserCmd* Command)
{
	static Aimbot* AimbotFeature = (Aimbot*)g_Features->Get("Aimbot");

	if (AimbotFeature)
		AimbotFeature->OnCreateMove(Command);

	return oCreateMove(_this, SampleTime, Command);
}

void CreateMove::Create()
{
	uintptr_t CreateMoveAddr = Memory::FindSignature("client.dll", CL_CreateMove);

	MH_CreateHook((LPVOID)CreateMoveAddr, (LPVOID)&hkCreateMove, (LPVOID*)&oCreateMove);
}

void CreateMove::Destroy()
{
}
