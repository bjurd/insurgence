#pragma once

#include "../features.h"
#include "../valve/cusercmd.h"

class Aimbot : public Feature
{
public:
	void Create();
	void Destroy();

public:
	void OnCreateMove(CUserCmd* Command);
};
