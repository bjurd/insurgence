#pragma once

#include "vector.h"

class CUserCmd
{
public:
	virtual ~CUserCmd() = 0;

	int CommandNumber;
	int TickCount;

	Angle ViewAngles;

	float ForwardMove;
	float SideMove;
	float UpMove;

	int Buttons;

	char Impulse;

	int WeaponSelect;
	int WeaponSubType;

	int RandomSeed;

	short MouseX;
	short MouseY;

	bool HasBeenPredicted;
};