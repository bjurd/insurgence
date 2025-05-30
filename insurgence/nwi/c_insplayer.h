#pragma once

#include "../valve/c_baseentity.h"
#include "../signatures.h"
#include "../netvars.h"

// C_BasePlayer too
class C_INSPlayer : public C_BaseEntity
{
public:
	NVPROXY(int, GetTeam, "DT_BaseEntity->m_iTeamNum");
	NVPROXY(int, GetHealth, "DT_BasePlayer->m_iHealth");
	NVPROXY(int, GetFlags, "DT_BasePlayer->m_fFlags");
	NVPROXY(int, GetFOV, "DT_BasePlayer->m_iFOV");
	NVPROXY(int, GetDefaultFOV, "DT_BasePlayer->m_iDefaultFOV");

	NVPROXY(int, GetMaxHealth, "DT_INSPlayer->m_iMaxHealth");

	CLPROXY(GetPlayerName, C_BasePlayer_GetPlayerName, const char*, (void));
};
