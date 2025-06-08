#pragma once

#include "../features.h"
#include "../valve/cusercmd.h"
#include <unordered_map>
#include <vector>

class C_INSPlayer;
class Vector;

class Aimbot : public Feature
{
public:
	bool Enabled = true;

	bool TargetHead = true;
	bool TargetTorso = true;
	bool TargetLimbs = false;

public:
	void Create();
	void Destroy();

public:
	bool ShouldAimAtGroup(int Hitgroup);

	std::unordered_map<int, std::vector<Vector>> GetHitgroupPositions(C_INSPlayer* Target);
	Vector GetIdealAimPosition(C_INSPlayer* Target, const std::unordered_map<int, std::vector<Vector>>& HitgroupPositions);
	Vector GetTargetAimPosition(C_INSPlayer* Target);

	C_INSPlayer* GetTarget();
	Vector GetAimPosition();

	void OnCreateMove(CUserCmd* Command);
};
