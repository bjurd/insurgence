#include "features.h"

#include "globals.h"

#include "features/aimbot.h"
#include "features/esp.h"

bool Features::Create()
{
	this->List["Aimbot"] = new Aimbot();
	this->List["ESP"] = new ESP();

	for (auto Current : this->List)
		Current.second->Create();

	return true;
}

void Features::Destroy()
{
	for (auto Current : this->List)
		Current.second->Destroy();
}

Feature* Features::Get(std::string Name)
{
	return this->List[Name];
}