#pragma once

#include "../valve/c_baseentity.h"
#include "../signatures.h"

class C_INSPlayer : public C_BaseEntity
{
public:
	PROXYVAR(GetTeam, int, 0x0120);
	PROXYVAR(GetHealth, int, 0x012C);
	PROXYVAR(GetFlags, char, 0x0130); // IsOnGround?

	CLPROXY(GetPlayerName, C_BasePlayer_GetPlayerName, const char*, (void));
};
