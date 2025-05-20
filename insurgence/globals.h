#pragma once

#include "memory.h"
#include "console.h"
#include "hooks.h"

class Insurgence
{
public:
	Insurgence()
	{
		this->MemoryManager = new Memory();
		this->ExternalConsole = new Console();
		this->HooksManager = new Hooks();
	}

	~Insurgence()
	{
		delete this->MemoryManager;
		delete this->ExternalConsole;
		delete this->HooksManager;
	}

public:
	Memory* MemoryManager;
	Console* ExternalConsole;
	Hooks* HooksManager;
};

inline Insurgence* Globals;