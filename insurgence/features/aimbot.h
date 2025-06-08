#pragma once

#include "../features.h"
#include "../valve/cusercmd.h"
#include <unordered_map>
#include <utility>
#include <vector>

class C_INSPlayer;
class Vector;

class Aimbot : public Feature
{
public:
	bool Enabled = true;

	float FOV = 16.f;

	bool TargetHead = true;
	bool TargetTorso = true;
	bool TargetLimbs = false;

public:
	void Create();
	void Destroy();

public:
	std::pair<bool, float> PositionInFOV(const Vector& Position);
	float DistanceFromCrosshair(const Vector& Position);
	float DistanceFromCrosshair(C_INSPlayer* Target);

	bool ShouldAimAtGroup(int Hitgroup);

	std::unordered_map<int, std::vector<Vector>> GetHitgroupPositions(C_INSPlayer* Target);
	Vector GetIdealAimPosition(C_INSPlayer* Target, const std::unordered_map<int, std::vector<Vector>>& HitgroupPositions);
	Vector GetTargetAimPosition(C_INSPlayer* Target);

	std::pair<C_INSPlayer*, Vector> GetTarget();
	Vector GetAimPosition();

	void OnCreateMove(CUserCmd* Command);
};
