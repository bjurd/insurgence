#pragma once

#include "ihandleentity.h"

class Vector;

class ICollideable
{
public:
	virtual IHandleEntity* GetEntityHandle() = 0;

	// NWI Removed these for some reason
	/*virtual const Vector& OBBMinsPreScaled() const = 0;
	virtual const Vector& OBBMaxsPreScaled() const = 0;*/
	virtual const Vector& OBBMins() const = 0;
	virtual const Vector& OBBMaxs() const = 0;
};
