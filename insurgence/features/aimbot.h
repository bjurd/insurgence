#pragma once

#include "../features.h"
#include "../valve/cusercmd.h"

class C_INSPlayer;
class Vector;

class Aimbot : public Feature
{
public:
	void Create();
	void Destroy();

public:
	Vector GetTargetAimPosition(C_INSPlayer* Target);
	Vector GetAimbotTarget();

	void OnCreateMove(CUserCmd* Command);
};
