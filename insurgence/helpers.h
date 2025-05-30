#pragma once

#include "globals.h"
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
	C_BaseEntity* GetLocalPlayerEntity();
	C_INSPlayer* GetLocalPlayer();

	inline PlayerRange PlayerIterator()
	{
		return PlayerRange();
	}
};
