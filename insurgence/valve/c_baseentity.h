#pragma once

#include "../signatures.h"
#include "../vmt.h"
#include "datamap.h"
#include "icliententity.h"

class ICollideable;
class Vector;
class Angle;

#undef GetClassName

class C_BaseEntity : public IClientEntity
{
public:
	// 0x48, 0x50, 0x8C
	PROXYVAR(EntIndex, int, 0x008C);

public:
	CLPROXY(GetAbsOrigin, C_BaseEntity_GetAbsOrigin, Vector&, (void));
	CLPROXY(GetAbsAngles, C_BaseEntity_GetAbsAngles, Angle&, (void));

	VPROXY(GetPredDescMap, 17, datamap_t*, (void));
	const char* GetClassName();

	CLPROXY(GetCollideable, C_BaseEntity_GetCollideable, ICollideable*, (void));

	// these crash the game for some reason
	// VPROXY(IsDormant, 78, bool, (void));
	// VPROXY(IsPlayer, 207, bool, (void));

	// get health 169
	// get max health 170
	// start touch 200
};
