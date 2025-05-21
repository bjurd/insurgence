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
		this->ExternalConsole = new Console();
		this->HooksManager = new Hooks();
		this->PointersManager = new Pointers();
	}

	~Insurgence()
	{
		delete this->ExternalConsole;
		delete this->HooksManager;
		delete this->PointersManager;
	}

public:
	Console* ExternalConsole;
	Hooks* HooksManager;
	Pointers* PointersManager;
};

inline Insurgence* Globals;