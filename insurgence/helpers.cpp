#include "helpers.h"

#include "globals.h"
#include "valve/c_baseentity.h"
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
