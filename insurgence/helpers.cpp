#include "helpers.h"

#include "globals.h"

C_BaseEntity* Helpers::GetLocalPlayerEntity()
{
	return (C_BaseEntity*)Globals->PointersManager->EntityList->GetClientEntity(Globals->PointersManager->Client->GetLocalPlayerIndex());
}

C_INSPlayer* Helpers::GetLocalPlayer()
{
	return (C_INSPlayer*)Globals->PointersManager->EntityList->GetClientEntity(Globals->PointersManager->Client->GetLocalPlayerIndex());
}
