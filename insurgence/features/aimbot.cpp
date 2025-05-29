#include "aimbot.h"

#include "../globals.h"
#include "../helpers.h"
#include "../nwi/c_insplayer.h"
#include "../signatures.h"
#include "../valve/math.h"
#include "../valve/studio.h"
#include "../valve/render.h"
#include "../valve/gametrace.h"
#include "../valve/const.h"
#include "../valve/bspflags.h"

void Aimbot::Create()
{
}

void Aimbot::Destroy()
{
}

Vector GetTargetAimPosition(C_INSPlayer* Target)
{
	Vector AimPos;
	AimPos.Invalidate();

	IClientRenderable* Renderable = Target->GetClientRenderable();
	if (!Renderable) return AimPos;

	const model_t* Model = Renderable->GetModel();
	if (!Model) return AimPos;

	studiohdr_t* Studio = Globals->PointersManager->ModelInfo->GetStudiomodel(Model);
	if (!Studio) return AimPos;
	if (Studio->numhitboxsets < 1) return AimPos;

	matrix3x4_t BoneMatrices[MAXSTUDIOBONES];

	if (!Target->SetupBonesReal(BoneMatrices, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, 0.f))
		return AimPos;

	C_INSPlayer* LocalPlayer = Helpers::GetLocalPlayer();
	Vector ViewOrigin = GetMainViewOrigin();

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

			Origin += ((Mins + Maxs) / 2.f);

			Ray_t Ray;
			Ray.Init(ViewOrigin, Origin);

			CTraceFilterSimple TraceFilter(LocalPlayer, COLLISION_GROUP_NONE);

			CGameTrace Result;
			Globals->PointersManager->EngineTrace->TraceRay(Ray, MASK_SHOT, &TraceFilter, &Result);

			if (Result.DidHit() && Result.m_pEnt == Target)
				return Origin;
		}
	}

	return AimPos;
}

Vector GetAimbotTarget()
{
	Vector AimPos;
	AimPos.Invalidate();

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
			Vector CurrentAimPos = GetTargetAimPosition(Player);

			if (CurrentAimPos.IsValid())
			{
				AimPos = CurrentAimPos;
				Distance = CurrentDistance;
			}
		}
	}

	return AimPos;
}

void Aimbot::OnCreateMove(CUserCmd* Command)
{
	if (GetAsyncKeyState(VK_XBUTTON2) && Command->CommandNumber > 0 && Command->TickCount > 0)
	{
		Vector AimPos = GetAimbotTarget();

		if (AimPos.IsValid())
		{
			// C_INSPlayer* LocalPlayer = Helpers::GetLocalPlayer();

			Vector AimDir = AimPos - GetMainViewOrigin(); // LocalPlayer->GetAbsOrigin();

			Angle AimAng;
			VectorAngles(AimDir, AimAng);

			Command->ViewAngles = AimAng;
		}
	}
}
