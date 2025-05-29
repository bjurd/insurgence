#pragma once

#include "trace.h"
#include "cmodel.h"
#include "c_baseentity.h"

class CGameTrace : public CBaseTrace
{
public:
	float		fractionleftsolid;
	csurface_t	surface;
	int			hitgroup;
	short		physicsbone;

	C_BaseEntity* m_pEnt;

	int			hitbox;

public: // If there's too many of these I'll make a .cpp
	bool DidHit()
	{
		return this->fraction < 1 || this->allsolid || this->startsolid;
	}
};
