#pragma once

#include "pointers.h"
#include "valve/client.h"
#include <string>

class C_BaseEntity;
class C_INSPlayer;

class BasePlayerIterator
{
public:
	int Index;

public:
	BasePlayerIterator(int Index);

	C_INSPlayer* operator*() const;
	BasePlayerIterator& operator++();
	bool operator!=(const BasePlayerIterator& Other) const;

private:
	virtual bool IsValid(C_INSPlayer* Player);
	void Advance();
};

class TargetPlayerIterator : public BasePlayerIterator
{
public:
	using BasePlayerIterator::BasePlayerIterator;

private:
	virtual bool IsValid(C_INSPlayer* Player);
};

template <typename T>
	requires std::is_base_of_v<BasePlayerIterator, T>
class PlayerRange
{
public:
	T begin()
	{
		return T(1);
	}

	T end()
	{
		return T(g_Pointers->EngineClient->GetMaxClients() + 1);
	}
};

namespace Helpers
{
	C_BaseEntity* GetLocalPlayerEntity();
	C_INSPlayer* GetLocalPlayer();

	namespace Iterators
	{
		inline PlayerRange<BasePlayerIterator> AllPlayers()
		{
			return PlayerRange<BasePlayerIterator>();
		}

		inline PlayerRange<TargetPlayerIterator> TargetPlayers()
		{
			return PlayerRange<TargetPlayerIterator>();
		}
	}
};
