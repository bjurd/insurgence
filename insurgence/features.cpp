#include "features.h"

#include "features/aimbot.h"
#include "features/esp.h"
#include "features/menu.h"

bool Features::Create()
{
	this->List = std::unordered_map<std::type_index, Feature*>();
	this->List[std::type_index(typeid(Aimbot))] = new Aimbot();
	this->List[std::type_index(typeid(ESP))] = new ESP();
	this->List[std::type_index(typeid(Menu))] = new Menu();

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
