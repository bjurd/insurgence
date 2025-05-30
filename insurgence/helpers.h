#pragma once

#include "globals.h"
#include <string>

#undef GetClassName

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
		return T(Globals->PointersManager->EntityList->GetHighestEntityIndex());
	}
};

namespace Helpers
{
	C_BaseEntity* GetLocalPlayerEntity();
	C_INSPlayer* GetLocalPlayer();

	inline PlayerRange<BasePlayerIterator> PlayerIterator()
	{
		return PlayerRange<BasePlayerIterator>();
	}

	inline PlayerRange<TargetPlayerIterator> TargetsPlayerIterator()
	{
		return PlayerRange<TargetPlayerIterator>();
	}
};
