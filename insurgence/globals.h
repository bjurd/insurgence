#pragma once

#include "memory.h"

#include "console.h"
#include "hooks.h"
#include "pointers.h"
#include "features.h"

class Insurgence
{
public:
	Insurgence();
	~Insurgence();

public:
	Console* ExternalConsole;
	Hooks* HooksManager;
	Pointers* PointersManager;
	Features* FeaturesManager;
};

inline Insurgence* Globals;
