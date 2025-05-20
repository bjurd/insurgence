#pragma once

#include "memory.h"
#include "console.h"

class Insurgence
{
public:
	Insurgence()
	{
		this->MemoryManager = new Memory();
		this->ExternalConsole = new Console();
	}

public:
	Memory* MemoryManager;
	Console* ExternalConsole;
};

inline Insurgence* Globals;