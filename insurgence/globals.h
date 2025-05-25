#pragma once

#include "memory.h"
#include "console.h"
#include "hooks.h"
#include "pointers.h"
#include "features.h"

class Insurgence
{
public:
	Insurgence()
	{
		this->ExternalConsole = new Console();
		this->HooksManager = new Hooks();
		this->PointersManager = new Pointers();
		this->FeaturesManager = new Features();
		
	}

	~Insurgence()
	{
		delete this->ExternalConsole;
		delete this->HooksManager;
		delete this->PointersManager;
		delete this->FeaturesManager;
	}

public:
	Console* ExternalConsole;
	Hooks* HooksManager;
	Pointers* PointersManager;
	Features* FeaturesManager;
};

inline Insurgence* Globals;