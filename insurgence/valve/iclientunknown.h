#pragma once

#include "ihandleentity.h"

class C_BaseEntity;

class IClientUnknown : public IHandleEntity
{
public:
	virtual void* GetCollideable() = 0;
	virtual void* GetClientNetworkable() = 0;
	virtual void* GetClientRenderable() = 0;
	virtual void* GetIClientEntity() = 0;
	virtual C_BaseEntity* GetBaseEntity() = 0;
	virtual void* GetClientThinkable() = 0;
};