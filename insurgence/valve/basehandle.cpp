#include "basehandle.h"

#include "../pointers.h"

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
	return this->IsInitialized() && g_Pointers->EntityList->GetClientEntity(this->GetEntryIndex()) != nullptr;
}
