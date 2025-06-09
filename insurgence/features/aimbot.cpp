#include "aimbot.h"

#include "../binds.h"
#include "../cache.h"
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
#include <algorithm>
#include <array>
#include <atomic>
#include <format>

const static std::array<int, 7> HitgroupOrder = { HITGROUP_HEAD, HITGROUP_CHEST, HITGROUP_STOMACH, HITGROUP_LEFTARM, HITGROUP_RIGHTARM, HITGROUP_LEFTLEG, HITGROUP_RIGHTLEG };

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

float Aimbot::GetFOVRadius()
{
	if (this->FOV <= 0.f)
		return 0.f;

	int ScreenCenterX = Cache::ViewSetup.Width / 2;

	float RenderFOV = Cache::ViewSetup.FOV;
	float VisualFOV = DEG2RAD(this->FOV / 2.f);

	return ScreenCenterX * tanf(VisualFOV) / tanf(DEG2RAD(RenderFOV) / 2.f);
}

std::pair<bool, float> Aimbot::PositionInFOV(const Vector& Position)
{
	float FOV = this->FOV;

	if (FOV <= 0.f)
		FOV = 360.f;

	Vector ViewDirection;
	AngleVectors(Cache::ViewSetup.Angles, ViewDirection);

	Vector Direction;
	VectorSubtract(Position, Cache::ViewSetup.Origin, Direction);
	VectorNormalize(Direction);

	float Dot = DotProduct(ViewDirection, Direction);
	float Adjusted = cosf(DEG2RAD(FOV) / 2.f);

	return std::make_pair<bool, float>(Dot >= Adjusted, Dot / Direction.Length());
}

float Aimbot::DistanceFromCrosshair(const Vector& Position)
{
	std::pair<bool, float> Result = this->PositionInFOV(Position);

	if (Result.first)
		return Result.second;

	return -1.f;
}

float Aimbot::DistanceFromCrosshair(C_INSPlayer* Target) // TODO: Pass HitgroupPositions in
{
	std::unordered_map<int, std::vector<Vector>> HitgroupPositions = this->GetHitgroupPositions(Target);

	for (int Hitgroup : HitgroupOrder)
	{
		auto Found = HitgroupPositions.find(Hitgroup);

		if (Found != HitgroupPositions.end())
		{
			for (const Vector& Position : Found->second)
			{
				float Distance = this->DistanceFromCrosshair(Position);

				if (Distance >= 0.f)
					return Distance;
			}
		}
	}

	return -1.f;
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
			if (Origin.IsValid())
			{
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
	}

	return Positions;
}

Vector Aimbot::GetIdealAimPosition(C_INSPlayer* Target, const std::unordered_map<int, std::vector<Vector>>& HitgroupPositions)
{
	Vector AimPos;
	AimPos.Invalidate();

	if (HitgroupPositions.empty())
		return AimPos;

	C_INSPlayer* LocalPlayer = Helpers::GetLocalPlayer();

	Ray_t Ray;
	CTraceFilterSimple TraceFilter(LocalPlayer, COLLISION_GROUP_NONE);

	for (int Hitgroup : HitgroupOrder)
	{
		auto Found = HitgroupPositions.find(Hitgroup);

		if (Found != HitgroupPositions.end())
		{
			for (const Vector& Position : Found->second)
			{
				Ray.Init(Cache::ViewSetup.Origin, Position);

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

std::pair<C_INSPlayer*, Vector> Aimbot::GetTarget()
{
	struct TargetEntry // Avoid "this" in lambda
	{
		C_INSPlayer* Player;
		float Distance;
	};

	std::vector<TargetEntry> Targets;
	Targets.reserve(g_Pointers->EngineClient->GetMaxClients() + 1);

	for (C_INSPlayer* Player : Helpers::Iterators::TargetPlayers())
	{
		if (!Player) continue;

		float Distance = this->DistanceFromCrosshair(Player);

		if (Distance < 0.f)
			continue;

		Targets.push_back(TargetEntry{ Player, Distance });
	}

	std::sort(Targets.begin(), Targets.end(),
		[](const TargetEntry& A, const TargetEntry& B)
		{
			return A.Distance < B.Distance;
		});

	for (const auto& Entry : Targets)
	{
		Vector AimPos = this->GetTargetAimPosition(Entry.Player);

		if (AimPos.IsValid())
			return { Entry.Player, AimPos }; // make_pair shits bricks when used here
	}

	Vector AimPos;
	AimPos.Invalidate();

	return { nullptr, AimPos }; // Here too
}

Vector Aimbot::GetAimPosition()
{
	auto [Target, AimPos] = this->GetTarget();

	return AimPos;
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
			Vector AimDir = AimPos - Cache::ViewSetup.Origin;

			Angle AimAng;
			VectorAngles(AimDir, AimAng);

			Command->ViewAngles = AimAng;
		}
	}
}
