#pragma once

#include "globals.h"
#include "valve/dt_recv.h"
#include <unordered_map>

class C_BaseEntity;
class C_INSPlayer;

class PlayerIterator
{
public:
	int Index;

public:
	PlayerIterator(int Index)
	{
		this->Index = Index;
		this->Advance();
	}

	C_INSPlayer* operator*() const
	{
		C_INSPlayer* Player = (C_INSPlayer*)Globals->PointersManager->EntityList->GetClientEntity(Index);
		return Player;
	}

	PlayerIterator& operator++()
	{
		Index++;
		this->Advance();

		return *this;
	}

	bool operator!=(const PlayerIterator& Other) const
	{
		return Index != Other.Index;
	}

private:
	void Advance()
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
};

class PlayerRange
{
public:
	PlayerIterator begin()
	{
		return PlayerIterator(1);
	}

	PlayerIterator end()
	{
		return PlayerIterator(Globals->PointersManager->EntityList->GetHighestEntityIndex());
	}
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
