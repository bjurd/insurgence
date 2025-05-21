#pragma once

#include "../vmt.hpp"
#include "icliententity.h"
#include "vector.h"

class C_BaseEntity : public IClientEntity
{
public:
	//VPROXY(GetAbsOrigin, 10, Vector&, (void));
	SIGPROXY(GetAbsOrigin, "client.dll", "40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 48 8D 83 ? ? ? ? 48 83 C4 ? 5B C3 CC CC CC CC CC 48 89 5C 24", Vector&, (void))
	VPROXY(GetAbsAngles, 11, Angle&, (void));
};
