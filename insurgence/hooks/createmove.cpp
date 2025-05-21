#include "createmove.h"

#include "../globals.h"
#include "../valve/cusercmd.h"
#include "../minhook/MinHook.h"
#include "../valve/math.h"

typedef bool (*fnCreateMove)(void* _this, float SampleTime, CUserCmd* Command);
fnCreateMove oCreateMove;

bool __fastcall hkCreateMove(void* _this, float SampleTime, CUserCmd* Command)
{
	/*IClientEntity* test = Globals->PointersManager->EntityList->GetClientEntity(2);

	if (test)
	{
		Vector RealPos = test->GetAbsOrigin();
		Vector ScreenPos;
		
		if (ScreenTransform(RealPos, ScreenPos))
		{
			printf("got 2's pos from %f %f %f at %f %f %f\n", RealPos.x, RealPos.y, RealPos.z, ScreenPos.x, ScreenPos.y, ScreenPos.z);
		}
	}*/

	/*Angle yes;
	Globals->PointersManager->Client->GetViewAngles(yes);

	printf("%f %f %f\n", yes.p, yes.y, yes.r);*/

	C_BaseEntity* pfuck = (C_BaseEntity*)Globals->PointersManager->EntityList->GetClientEntity(Globals->PointersManager->Client->GetLocalPlayerIndex());

	if (pfuck)
	{
		Vector pos = pfuck->GetAbsOrigin();
		printf("%f %f %f\n", pos.x, pos.y, pos.z);
	}

	return oCreateMove(_this, SampleTime, Command);
}

void CreateMove::Create()
{
	uintptr_t CreateMoveAddr = Memory::FindSignature("client.dll", "40 53 48 83 EC ? 83 C9 ? 0F 29 74 24");
	//uintptr_t CL_MoveAddr = Globals->MemoryManager->FindSignature("engine.dll", "48 89 5C 24 ? 57 48 83 EC ? 0F 29 74 24 ? 0F B6 DA");
	//uintptr_t CL_SendMoveAddr = Globals->MemoryManager->FindSignature("engine.dll", "48 89 5C 24 ? 57 B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? E8");

	MH_CreateHook((LPVOID)CreateMoveAddr, (LPVOID)&hkCreateMove, (LPVOID*)&oCreateMove);
}

void CreateMove::Destroy()
{
	
}