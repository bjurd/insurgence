#include "features.h"

#include "globals.h"

#include "features/aimbot.h"
#include "features/esp.h"
#include "features/menu.h"

bool Features::Create()
{
	this->List = std::unordered_map<std::string, Feature*>();
	this->List["Aimbot"] = new Aimbot();
	this->List["ESP"] = new ESP();
	this->List["Menu"] = new Menu();

	for (auto Current : this->List)
		Current.second->Create();

	return true;
}

void Features::Destroy()
{
	for (auto Current : this->List)
	{
		Current.second->Destroy();

		delete Current.second;
	}
	this->List.clear();
}

Feature* Features::Get(std::string Name)
{
	return this->List[Name];
}