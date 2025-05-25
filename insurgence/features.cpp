#include "features.h"

#include "globals.h"

#include "features/aimbot.h"

bool Features::Create()
{
	this->List["Aimbot"] = new Aimbot();

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