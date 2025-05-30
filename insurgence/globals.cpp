#pragma once

#include "globals.h"
#include "globals.h"

Insurgence::Insurgence()
{
	this->ExternalConsole = new Console();
	this->HooksManager = new Hooks();
	this->PointersManager = new Pointers();
	this->FeaturesManager = new Features();
}

Insurgence::~Insurgence()
{
	delete this->ExternalConsole;
	delete this->HooksManager;
	delete this->PointersManager;
	delete this->FeaturesManager;
}
