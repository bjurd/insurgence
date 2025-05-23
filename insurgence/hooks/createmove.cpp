#include "createmove.h"

#include "../globals.h"
#include "../valve/cusercmd.h"
#include "../minhook/MinHook.h"
#include "../valve/math.h"
#include "../signatures.h"

typedef bool (*fnCreateMove)(void* _this, float SampleTime, CUserCmd* Command);
fnCreateMove oCreateMove;

bool __fastcall hkCreateMove(void* _this, float SampleTime, CUserCmd* Command)
{
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