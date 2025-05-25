#include "aimbot.h"

#include "../globals.h"
#include "../helpers.h"
#include "../nwi/c_insplayer.h"
#include "../signatures.h"
#include "../valve/math.h"
#include "../valve/studio.h"
#include "../valve/render.h"

void Aimbot::Create()
{

}

void Aimbot::Destroy()
{

}

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

		C_BaseEntity* Entity = (C_BaseEntity*)Globals->PointersManager->EntityList->GetClientEntity(i);

		if (!Entity || strcmp(Entity->GetClassName(), "C_INSPlayer")) // TODO: IsPlayer
			continue;

		C_INSPlayer* Player = (C_INSPlayer*)Entity;

		if (*Player->GetHealth() <= 0)
			continue;

		IClientRenderable* Renderable = Player->GetClientRenderable();
		if (!Renderable) continue;

		const model_t* Model = Renderable->GetModel();
		if (!Model) continue;

		studiohdr_t* Studio = Globals->PointersManager->ModelInfo->GetStudiomodel(Model);
		if (!Studio) continue;

		float CurrentDistance = Player->GetAbsOrigin().DistToSqr(LocalPlayerOrigin);

		if (CurrentDistance < Distance)
		{
			Distance = CurrentDistance;
			Closest = Player;
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

	matrix3x4_t BoneMatrix[MAXSTUDIOBONES];

	if (!Target->SetupBonesReal(BoneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.f))
	{
		printf("Failed to setup bones\n");
		return Vector();
	}

	for (int i = 0; i < Studio->numbones; ++i)
	{
		mstudiobone_t* Bone = Studio->GetBone(i);

		if (!Bone || !(Bone->flags & BONE_USED_BY_HITBOX))
			continue;

		Vector Origin = Vector(BoneMatrix[i][0][3], BoneMatrix[i][1][3], BoneMatrix[i][2][3]);
		Angle Rotation = Bone->rot.ToAngle();

		return Origin;
	}

	return Vector();
}

void Aimbot::OnCreateMove(CUserCmd* Command)
{
	if (GetAsyncKeyState(VK_XBUTTON2) && Command->CommandNumber > 0 && Command->TickCount > 0)
	{
		C_INSPlayer* Target = GetAimbotTarget();

		if (Target)
		{
			C_INSPlayer* LocalPlayer = Helpers::GetLocalPlayer();

			Vector AimPos = GetTargetAimPosition(Target);
			Vector AimDir = AimPos - GetMainViewOrigin(); // LocalPlayer->GetAbsOrigin();

			Angle AimAng;
			VectorAngles(AimDir, AimAng);

			Command->ViewAngles = AimAng;
		}
	}
}
