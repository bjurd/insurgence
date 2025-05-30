#include "helpers.h"

#include "globals.h"
#include "valve/client_class.h"
#include <format>
#include <string>

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
