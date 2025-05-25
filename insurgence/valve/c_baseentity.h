#pragma once

#include "../signatures.h"
#include "../vmt.hpp"
#include "icliententity.h"
#include "vector.h"
#include "datamap.h"

#undef GetClassName

class C_BaseEntity : public IClientEntity
{
public:
	// 0x48, 0x50, 0x8C
	PROXYVAR(EntIndex, int, 0x008C)

public:
	CLPROXY(GetAbsOrigin, C_BaseEntity_GetAbsOrigin, Vector&, (void))
	CLPROXY(GetAbsAngles, C_BaseEntity_GetAbsAngles, Angle&, (void))
	VPROXY(GetPredDescMap, 17, datamap_t*, (void))

	const char* GetClassName()
	{
		return this->GetPredDescMap()->dataClassName;
	}

	CLPROXY(GetCollideable, C_BaseEntity_GetCollideable, ICollideable*, (void))
};
