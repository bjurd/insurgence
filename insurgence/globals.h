#pragma once

#include "memory.h"

class Insurgence
{
public:
	Insurgence()
	{
		this->MemoryManager = new Memory();
	}

public:
	Memory* MemoryManager;
};

inline Insurgence* Globals;