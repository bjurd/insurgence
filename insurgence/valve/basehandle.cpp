#include "basehandle.h"

#include "../globals.h"

bool CBaseHandle::IsInitialized()
{
	return this->m_Index != INVALID_EHANDLE_INDEX;
}

int CBaseHandle::GetEntryIndex()
{
	return this->m_Index & ENT_ENTRY_MASK;
}

int CBaseHandle::GetSerialNumber()
{
	return this->m_Index >> NUM_ENT_ENTRY_BITS;
}

bool CBaseHandle::IsValid()
{
	return this->IsInitialized() && Globals->PointersManager->EntityList->GetClientEntity(this->GetEntryIndex()) != nullptr;
}
