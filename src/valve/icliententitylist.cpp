#pragma once

#include "icliententitylist.h"

#include "basehandle.h"
#include "iclientunknown.h"

C_BaseEntity* IClientEntityList::GetBaseEntityFromHandle(CBaseHandle Handle)
{
	IClientUnknown* Ent = GetClientUnknownFromHandle(Handle);

	return Ent ? Ent->GetBaseEntity() : 0;
}
