#pragma once

#include "memory.h"
#include "console.h"
#include "hooks.h"
#include "pointers.h"

class Insurgence
{
public:
	Insurgence()
	{
		this->MemoryManager = new Memory();
		this->ExternalConsole = new Console();
		this->HooksManager = new Hooks();
		this->PointersManager = new Pointers();
	}

	~Insurgence()
	{
		delete this->MemoryManager;
		delete this->ExternalConsole;
		delete this->HooksManager;
		delete this->PointersManager;
	}

public:
	Memory* MemoryManager;
	Console* ExternalConsole;
	Hooks* HooksManager;
	Pointers* PointersManager;
};

inline Insurgence* Globals;