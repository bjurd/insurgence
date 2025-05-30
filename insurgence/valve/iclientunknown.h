#pragma once

#include "ihandleentity.h"

class ICollideable;
class IClientRenderable;
class C_BaseEntity;

class IClientUnknown : public IHandleEntity
{
public:
	virtual ICollideable* _GetCollideable() = 0; // Might be right, sigging it anyway
	virtual void* GetClientNetworkable() = 0;
	virtual IClientRenderable* GetClientRenderable() = 0;
	virtual void* GetIClientEntity() = 0;
	virtual C_BaseEntity* GetBaseEntity() = 0;
	virtual void* GetClientThinkable() = 0;
};
