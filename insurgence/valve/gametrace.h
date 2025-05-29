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
};
