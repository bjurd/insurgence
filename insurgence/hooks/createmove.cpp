#include "createmove.h"

#include "../globals.h"
#include "../valve/cusercmd.h"
#include "../minhook/MinHook.h"
#include "../valve/math.h"
#include "../signatures.h"
#include "../nwi/c_insplayer.h"
#include "../valve/studio.h"

typedef bool (*fnCreateMove)(void* _this, float SampleTime, CUserCmd* Command);
fnCreateMove oCreateMove;

bool __fastcall hkCreateMove(void* _this, float SampleTime, CUserCmd* Command)
{
	C_BaseEntity* LocalPlayer = (C_BaseEntity*)Globals->PointersManager->EntityList->GetClientEntity(Globals->PointersManager->Client->GetLocalPlayerIndex());

	if (!LocalPlayer)
		return oCreateMove(_this, SampleTime, Command);

	int Entities = Globals->PointersManager->EntityList->GetHighestEntityIndex();

	for (int i = 1; i < Entities; ++i)
	{
		C_BaseEntity* Ent = (C_BaseEntity*)Globals->PointersManager->EntityList->GetClientEntity(i);

		if (!Ent)
			continue;

		if (strcmp(Ent->GetClassName(), "C_INSPlayer")) // TODO: IsPlayer
			continue;

		IClientRenderable* Renderable = Ent->GetClientRenderable();
		
		if (!Renderable)
			continue;

		const model_t* Model = Renderable->GetModel();

		if (!Model)
			continue;

		studiohdr_t* Studio = Globals->PointersManager->ModelInfo->GetStudiomodel(Model);

		if (!Studio)
			continue;

		printf("got model for %d %s\n", i, Studio->name);
	}

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