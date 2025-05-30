#pragma once

#include "globals.h"
#include "valve/dt_recv.h"
#include <unordered_map>
#include <string>

#undef GetClassName

class C_BaseEntity;
class C_INSPlayer;

class PlayerIterator
{
public:
	int Index;

public:
	PlayerIterator(int Index);

	C_INSPlayer* operator*() const;
	PlayerIterator& operator++();
	bool operator!=(const PlayerIterator& Other) const;

private:
	void Advance();
};

class PlayerRange
{
public:
	PlayerIterator begin();
	PlayerIterator end();
};

namespace Helpers
{
	inline std::unordered_map<size_t, RecvProp*> NetVars;

	C_BaseEntity* GetLocalPlayerEntity();
	C_INSPlayer* GetLocalPlayer();

	inline PlayerRange PlayerIterator()
	{
		return PlayerRange();
	}

	void StoreRecvTable(RecvTable* Table);
	void LoadNetVars();

	template <typename T>
	T GetNetVar(uintptr_t Base, std::string VarName)
	{
		std::hash<std::string> Hash;
		RecvProp* Prop = Helpers::NetVars[Hash(VarName)];

		if (!Prop)
			return (T)NULL;

		int Offset = Prop->m_Offset;

		return *(T*)(Base + Offset);
	}
};

#define NVPROXY(Type, MethodName, VarName)							\
	Type MethodName()												\
	{																\
		return Helpers::GetNetVar<Type>((uintptr_t)this, VarName);	\
	}
