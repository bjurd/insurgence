#include "helpers.h"

#include "globals.h"
#include "valve/client_class.h"
#include "valve/c_baseentity.h"
#include <format>
#include <string>

PlayerIterator::PlayerIterator(int Index)
{
	this->Index = Index;
	this->Advance();
}

C_INSPlayer* PlayerIterator::operator*() const
{
	C_INSPlayer* Player = (C_INSPlayer*)Globals->PointersManager->EntityList->GetClientEntity(Index);
	return Player;
}

PlayerIterator& PlayerIterator::operator++()
{
	Index++;
	this->Advance();

	return *this;
}

void PlayerIterator::Advance()
{
	int Entities = Globals->PointersManager->EntityList->GetHighestEntityIndex();

	while (Index < Entities)
	{
		C_BaseEntity* Entity = (C_BaseEntity*)Globals->PointersManager->EntityList->GetClientEntity(Index);

		if (!Entity || strcmp(Entity->GetClassName(), "C_INSPlayer") != 0)
		{
			Index++;
			continue;
		}

		break;
	}
}

PlayerIterator PlayerRange::begin()
{
	return PlayerIterator(1);
}

PlayerIterator PlayerRange::end()
{
	return PlayerIterator(Globals->PointersManager->EntityList->GetHighestEntityIndex());
}

bool PlayerIterator::operator!=(const PlayerIterator& Other) const
{
	return Index != Other.Index;
}

C_BaseEntity* Helpers::GetLocalPlayerEntity()
{
	return (C_BaseEntity*)Globals->PointersManager->EntityList->GetClientEntity(Globals->PointersManager->Client->GetLocalPlayerIndex());
}

C_INSPlayer* Helpers::GetLocalPlayer()
{
	return (C_INSPlayer*)Globals->PointersManager->EntityList->GetClientEntity(Globals->PointersManager->Client->GetLocalPlayerIndex());
}

void Helpers::StoreRecvTable(RecvTable* Table)
{
	std::hash<std::string> Hash;

	for (int i = 0; i < Table->m_nProps; ++i)
	{
		RecvProp* Prop = &Table->m_pProps[i];

		if (!Prop || isdigit(Prop->m_pVarName[0])) // homonovus!
			continue;

		if (Prop->m_RecvType == DPT_DataTable && Prop->m_pDataTable && Prop->m_pDataTable->m_pNetTableName[0] == 'D')
			Helpers::StoreRecvTable(Prop->m_pDataTable);

		std::string Name = std::format("{}->{}", Table->m_pNetTableName, Prop->m_pVarName);

		std::cout << Name << std::endl;

		Helpers::NetVars[Hash(Name)] = Prop;
	}
}

void Helpers::LoadNetVars()
{
	ClientClass* Classes = Globals->PointersManager->Client->GetAllClasses();

	for (ClientClass* Class = Classes; Class != nullptr; Class = Class->m_pNext)
		Helpers::StoreRecvTable(Class->m_pRecvTable);
}
