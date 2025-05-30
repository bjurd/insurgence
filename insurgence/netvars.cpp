#include "netvars.h"

#include "globals.h"
#include "valve/client_class.h"
#include <format>

void NetVars::StoreRecvTable(RecvTable* Table)
{
	std::hash<std::string> Hash;

	for (int i = 0; i < Table->m_nProps; ++i)
	{
		RecvProp* Prop = &Table->m_pProps[i];

		if (!Prop || isdigit(Prop->m_pVarName[0])) // homonovus!
			continue;

		if (Prop->m_RecvType == DPT_DataTable && Prop->m_pDataTable && Prop->m_pDataTable->m_pNetTableName[0] == 'D')
			NetVars::StoreRecvTable(Prop->m_pDataTable);

		std::string Name = std::format("{}->{}", Table->m_pNetTableName, Prop->m_pVarName);

		NetVars::List[Hash(Name)] = Prop;
	}
}

void NetVars::Load()
{
	ClientClass* Classes = Globals->PointersManager->Client->GetAllClasses();

	for (ClientClass* Class = Classes; Class != nullptr; Class = Class->m_pNext)
		NetVars::StoreRecvTable(Class->m_pRecvTable);
}
