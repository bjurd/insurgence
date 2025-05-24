#include "createmove.h"

#include "../globals.h"
#include "../valve/cusercmd.h"
#include "../minhook/MinHook.h"
#include "../valve/math.h"
#include "../signatures.h"
#include "../nwi/c_insplayer.h"
#include "../valve/studio.h"
#include "../helpers.h"

typedef bool (*fnCreateMove)(void* _this, float SampleTime, CUserCmd* Command);
fnCreateMove oCreateMove;

C_INSPlayer* GetAimbotTarget()
{
	C_INSPlayer* Closest = nullptr;
	float Distance = FLT_MAX;

	C_INSPlayer* LocalPlayer = Helpers::GetLocalPlayer();
	Vector LocalPlayerOrigin = LocalPlayer->GetAbsOrigin();

	int Entities = Globals->PointersManager->EntityList->GetHighestEntityIndex();

	for (int i = 1; i < Entities; ++i)
	{
		if (i == Globals->PointersManager->Client->GetLocalPlayerIndex())
			continue;

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

		float CurrentDistance = Ent->GetAbsOrigin().DistToSqr(LocalPlayerOrigin);

		if (CurrentDistance < Distance)
		{
			Distance = CurrentDistance;
			Closest = (C_INSPlayer*)Ent;
		}
	}

	return Closest;
}

Vector GetTargetAimPosition(C_INSPlayer* Target)
{
	Vector AimPos;

	IClientRenderable* Renderable = Target->GetClientRenderable();
	if (!Renderable) return AimPos;

	const model_t* Model = Renderable->GetModel();
	if (!Model) return AimPos;

	studiohdr_t* Studio = Globals->PointersManager->ModelInfo->GetStudiomodel(Model);
	if (!Studio) return AimPos;

	matrix3x4_t Bones;
	Target->SetupBones(&Bones, Studio->numbones, BONE_USED_BY_ANYTHING_AT_LOD(0), 0.f);

	for (int i = 0; i < Studio->numbones; ++i)
	{
		mstudiobone_t* Bone = Studio->GetBone(i);

		if (!Bone || !(Bone->flags & BONE_USED_BY_HITBOX))
			continue;

		Vector Origin = Bone->pos;
		Angle Rotation = Bone->rot.ToAngle();

		std::cout << std::hex << Bone << std::endl;

		return Origin;
	}
}

bool __fastcall hkCreateMove(void* _this, float SampleTime, CUserCmd* Command)
{
	if (GetAsyncKeyState(VK_XBUTTON2) && Command->CommandNumber > 0 && Command->TickCount > 0)
	{
		C_INSPlayer* Target = GetAimbotTarget();

		if (Target)
		{
			C_INSPlayer* LocalPlayer = Helpers::GetLocalPlayer();

			Vector AimPos = GetTargetAimPosition(Target);
			Vector AimDir = AimPos - LocalPlayer->GetAbsOrigin();

			Angle AimAng;
			VectorAngles(AimDir, AimAng);

			Command->ViewAngles = AimAng;
		}
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