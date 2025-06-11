#pragma once

#define	MAX_EDICT_BITS				11 // May be wrong for this game, /shrug

#define NUM_ENT_ENTRY_BITS		(MAX_EDICT_BITS + 1)
#define NUM_ENT_ENTRIES			(1 << NUM_ENT_ENTRY_BITS)
#define ENT_ENTRY_MASK			(NUM_ENT_ENTRIES - 1)

#define INVALID_EHANDLE_INDEX 0xFFFFFFFF

class CBaseHandle
{
public:
	unsigned long m_Index;

public:
	bool IsInitialized();

	int GetEntryIndex();
	int GetSerialNumber();

	bool IsValid();
};
