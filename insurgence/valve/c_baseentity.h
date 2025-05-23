#pragma once

#include "../signatures.h"
#include "../vmt.hpp"
#include "icliententity.h"
#include "vector.h"

class C_BaseEntity : public IClientEntity
{
public:
	CLPROXY(GetAbsOrigin, C_BaseEntity_GetAbsOrigin, Vector&, (void))
	CLPROXY(GetAbsAngles, C_BaseEntity_GetAbsAngles, Angle&, (void))
};
