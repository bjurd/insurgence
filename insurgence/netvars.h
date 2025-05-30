#pragma once

#include "vmt.h"
#include "valve/dt_recv.h"
#include <unordered_map>
#include <string>

class RecvProp;

namespace NetVars
{
	inline std::unordered_map<size_t, RecvProp*> List;

	template <typename T>
	T* GetNetVar(void* Object, std::string VarName)
	{
		std::hash<std::string> Hash;
		RecvProp* Prop = NetVars::List[Hash(VarName)];

		if (!Prop)
			return nullptr;

		return VMT::GetVarAt<T>(Object, Prop->m_Offset);
	}

	void StoreRecvTable(RecvTable* Table);
	void Create();
	void Destroy();
}

#define NVPROXY(Type, MethodName, VarName)					\
	Type* MethodName()										\
	{														\
		return NetVars::GetNetVar<Type>(this, VarName);		\
	}
