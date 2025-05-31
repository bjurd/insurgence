#pragma once

#include "../signatures.h"
#include "../vmt.h"
#include "../netvars.h"
#include "datamap.h"
#include "icliententity.h"

class ICollideable;
class Vector;
class Angle;

#undef GetClassName

class C_BaseEntity : public IClientEntity
{
public:
	NVPROXY(int, GetTeam, "DT_BaseEntity->m_iTeamNum");

public:
	PROXYVAR(EntIndex, int, 0x008C);
	PROXYVAR(EntClientFlags, int, 0x0088);
	PROXYVAR(IsDormant, bool, 0x0109);

public:
	CLPROXY(GetAbsOrigin, C_BaseEntity_GetAbsOrigin, Vector&, (void));
	CLPROXY(GetAbsAngles, C_BaseEntity_GetAbsAngles, Angle&, (void));

	VPROXY(GetPredDescMap, 17, datamap_t*, (void));
	const char* GetClassName();

	CLPROXY(GetCollideable, C_BaseEntity_GetCollideable, ICollideable*, (void));
};
