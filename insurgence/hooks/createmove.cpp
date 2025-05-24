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

void AimAtPlayer(CUserCmd* Command, C_INSPlayer* Target, studiohdr_t* Studio, const model_t* Model)
{
	C_BaseEntity* LocalPlayer = (C_BaseEntity*)Globals->PointersManager->EntityList->GetClientEntity(Globals->PointersManager->Client->GetLocalPlayerIndex());

	matrix3x4_t Bones;
	Target->SetupBones(&Bones, Studio->numbones, BONE_USED_BY_ANYTHING_AT_LOD(0), 0.f);

	for (int i = 0; i < Studio->numbones; ++i)
	{
		mstudiobone_t* Bone = Studio->GetBone(i);

		if (!Bone || !(Bone->flags & BONE_USED_BY_HITBOX))
			continue;

		Vector Origin = Bone->pos;
		Angle Rotation = Bone->rot.ToAngle();

		Vector AimDir = Origin - LocalPlayer->GetAbsOrigin();

		Angle AimAng;
		VectorAngles(AimDir, AimAng);

		Command->ViewAngles = AimAng;
		return;
	}
}

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

		AimAtPlayer(Command, (C_INSPlayer*)Ent, Studio, Model);
		break;

		//printf("got model for %d %s\n", i, Studio->name);
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