#pragma once

#include "valve/c_baseentity.h"
#include "nwi/c_insplayer.h"

class Helpers
{
public:
	static C_BaseEntity* GetLocalPlayerEntity();
	static C_INSPlayer* GetLocalPlayer();
};