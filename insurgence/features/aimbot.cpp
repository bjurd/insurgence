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
	int LocalTeam = *LocalPlayer->GetTeam();

	for (C_INSPlayer* Player : Helpers::PlayerIterator())
	{
		if (!Player) continue;

		if (Player == LocalPlayer) continue;
		if (*Player->GetHealth() <= 0) continue;
		if (*Player->GetTeam() == LocalTeam) continue;

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
	if (Studio->numhitboxsets < 1) return AimPos;

	matrix3x4_t BoneMatrices[MAXSTUDIOBONES];

	if (!Target->SetupBonesReal(BoneMatrices, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, 0.f))
	{
		printf("Failed to setup bones\n");
		return Vector();
	}

	for (int s = 0; s < Studio->numhitboxsets; ++s)
	{
		mstudiohitboxset_t* HitboxSet = Studio->GetHitboxSet(s);

		if (!HitboxSet || HitboxSet->numhitboxes < 1)
			continue;

		for (int h = 0; h < HitboxSet->numhitboxes; ++h)
		{
			mstudiobbox_t* Hitbox = HitboxSet->GetHitbox(h);

			if (!Hitbox || Hitbox->group != HITGROUP_HEAD)
				continue;

			mstudiobone_t* Bone = Studio->GetBone(Hitbox->bone);

			if (!Bone || !(Bone->flags & BONE_USED_BY_HITBOX)) // Should never happen
				continue;

			VMatrix BoneMatrix = BoneMatrices[Hitbox->bone];

			Vector Origin;
			Angle Angles;

			BoneMatrix.GetTranslation(Origin);
			BoneMatrix.GetAngles(Angles);

			Vector Mins = Hitbox->bbmin;
			Vector Maxs = Hitbox->bbmax;
			VectorRotate(Mins, Angles, Mins);
			VectorRotate(Maxs, Angles, Maxs);

			return Origin + ((Mins + Maxs) / 2.f);
		}
	}

	return AimPos;
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
