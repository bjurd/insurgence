#pragma once

#include "../signatures.h"
#include "../vmt.hpp"
#include "icliententity.h"
#include "vector.h"

class C_BaseEntity : public IClientEntity
{
public:
	SIGPROXY(GetAbsOrigin, "client.dll", C_BaseEntity_GetAbsOrigin, Vector&, (void))
	SIGPROXY(GetAbsAngles, "client.dll", C_BaseEntity_GetAbsAngles, Angle&, (void))
};
