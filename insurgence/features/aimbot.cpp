#include "aimbot.h"

#include "../binds.h"
#include "../helpers.h"
#include "../nwi/c_insplayer.h"
#include "../pointers.h"
#include "../signatures.h"
#include "../valve/bspflags.h"
#include "../valve/const.h"
#include "../valve/gametrace.h"
#include "../valve/math.h"
#include "../valve/render.h"
#include "../valve/studio.h"
#include <atomic>
#include <format>
#include <array>

std::atomic<bool> CanActivate = false;

void Aimbot::Create()
{
	Binds::Create(VK_XBUTTON2, BindMode::Hold, std::function([](bool IsPressed, bool WasPressed)
		{
			CanActivate.store(IsPressed);
		}));
}

void Aimbot::Destroy()
{
}

bool Aimbot::ShouldAimAtGroup(int Hitgroup)
{
	switch (Hitgroup)
	{
	case HITGROUP_HEAD:
		return this->TargetHead;

	case HITGROUP_CHEST:
	case HITGROUP_STOMACH:
		return this->TargetTorso;

	case HITGROUP_LEFTARM:
	case HITGROUP_RIGHTARM:
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		return this->TargetLimbs;

	default:
		return false;
	}
}

std::unordered_map<int, std::vector<Vector>> Aimbot::GetHitgroupPositions(C_INSPlayer* Target)
{
	std::unordered_map<int, std::vector<Vector>> Positions; // Slightly sketch variable naming

	IClientRenderable* Renderable = Target->GetClientRenderable();
	if (!Renderable) return Positions;

	const model_t* Model = Renderable->GetModel();
	if (!Model) return Positions;

	studiohdr_t* Studio = g_Pointers->ModelInfo->GetStudiomodel(Model);
	if (!Studio) return Positions;
	if (Studio->numhitboxsets < 1) return Positions;

	matrix3x4_t BoneMatrices[MAXSTUDIOBONES];

	if (!Target->SetupBonesReal(BoneMatrices, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, 0.f))
		return Positions;

	for (int s = 0; s < Studio->numhitboxsets; ++s)
	{
		mstudiohitboxset_t* HitboxSet = Studio->GetHitboxSet(s);

		if (!HitboxSet || HitboxSet->numhitboxes < 1)
			continue;

		for (int h = 0; h < HitboxSet->numhitboxes; ++h)
		{
			mstudiobbox_t* Hitbox = HitboxSet->GetHitbox(h);

			if (!Hitbox || !this->ShouldAimAtGroup(Hitbox->group))
				continue;

			mstudiobone_t* Bone = Studio->GetBone(Hitbox->bone);

			if (!Bone || !(Bone->flags & BONE_USED_BY_HITBOX)) // Should never happen
				continue;

			// Move position to center of hitbox
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

			// Save
			auto Found = Positions.find(Hitbox->group);

			if (Found == Positions.end())
			{
				Positions[Hitbox->group] = std::vector<Vector>();
				Found = Positions.find(Hitbox->group);
			}

			std::vector<Vector>& HitgroupPositions = Found->second;
			HitgroupPositions.push_back(Origin);
		}
	}

	return Positions;
}

Vector Aimbot::GetIdealAimPosition(C_INSPlayer* Target, const std::unordered_map<int, std::vector<Vector>>& HitgroupPositions)
{
	Vector AimPos;
	AimPos.Invalidate();

	if (HitgroupPositions.empty())
		return AimPos;

	const static std::array<int, 7> HitgroupOrder = { HITGROUP_HEAD, HITGROUP_CHEST, HITGROUP_STOMACH, HITGROUP_LEFTARM, HITGROUP_RIGHTARM, HITGROUP_LEFTLEG, HITGROUP_RIGHTLEG };

	C_INSPlayer* LocalPlayer = Helpers::GetLocalPlayer();
	Vector ViewOrigin = GetMainViewOrigin();

	Ray_t Ray;
	CTraceFilterSimple TraceFilter(LocalPlayer, COLLISION_GROUP_NONE);

	for (int Hitgroup : HitgroupOrder)
	{
		auto Found = HitgroupPositions.find(Hitgroup);

		if (Found != HitgroupPositions.end())
		{
			for (const Vector& Position : Found->second)
			{
				Ray.Init(ViewOrigin, Position);

				CGameTrace Result;
				g_Pointers->EngineTrace->TraceRay(Ray, MASK_SHOT, &TraceFilter, &Result);

				if (Result.DidHit() && Result.m_pEnt == Target)
				{
					VectorCopy(Position, AimPos);
					return AimPos;
				}
			}
		}
	}

	return AimPos;
}

Vector Aimbot::GetTargetAimPosition(C_INSPlayer* Target)
{
	std::unordered_map<int, std::vector<Vector>> HitgroupPositions = this->GetHitgroupPositions(Target);

	return this->GetIdealAimPosition(Target, HitgroupPositions);
}

C_INSPlayer* Aimbot::GetTarget()
{
	C_INSPlayer* Target = nullptr;
	float Distance = FLT_MAX;

	Vector LocalPlayerOrigin = Helpers::GetLocalPlayer()->GetAbsOrigin();

	for (C_INSPlayer* Player : Helpers::Iterators::TargetPlayers())
	{
		if (!Player) continue;

		float CurrentDistance = Player->GetAbsOrigin().DistToSqr(LocalPlayerOrigin);

		if (CurrentDistance < Distance)
		{
			Vector CurrentAimPos = this->GetTargetAimPosition(Player);

			if (CurrentAimPos.IsValid())
			{
				Target = Player;
				Distance = CurrentDistance;
			}
		}
	}

	return Target;
}

Vector Aimbot::GetAimPosition()
{
	C_INSPlayer* Target = this->GetTarget();

	if (!Target)
	{
		Vector AimPos;
		AimPos.Invalidate();

		return AimPos;
	}

	return this->GetTargetAimPosition(Target);
}

void Aimbot::OnCreateMove(CUserCmd* Command)
{
	if (!this->Enabled)
		return;

	if (CanActivate.load() && Command->CommandNumber > 0 && Command->TickCount > 0)
	{
		Vector AimPos = this->GetAimPosition();

		if (AimPos.IsValid())
		{
			Vector AimDir = AimPos - GetMainViewOrigin();

			Angle AimAng;
			VectorAngles(AimDir, AimAng);

			Command->ViewAngles = AimAng;
		}
	}
}
