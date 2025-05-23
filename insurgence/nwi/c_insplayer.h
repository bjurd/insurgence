#pragma once

#include "../valve/c_baseentity.h"

class C_INSPlayer : public C_BaseEntity
{
public:
	PROXYVAR(GetHealth, int, 0x012C)
	PROXYVAR(GetFlags, char, 0x0130) // IsOnGround?
};
