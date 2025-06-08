#include "helpers.h"

#include "nwi/c_insplayer.h"
#include "pointers.h"
#include <string>

BasePlayerIterator::BasePlayerIterator(int Index)
{
	this->Index = Index;
	this->Advance();
}

C_INSPlayer* BasePlayerIterator::operator*() const
{
	C_INSPlayer* Player = static_cast<C_INSPlayer*>(g_Pointers->EntityList->GetClientEntity(Index));
	return Player;
}

BasePlayerIterator& BasePlayerIterator::operator++()
{
	Index++;
	this->Advance();

	return *this;
}

bool BasePlayerIterator::operator!=(const BasePlayerIterator& Other) const
{
	return this->Index != Other.Index;
}

bool BasePlayerIterator::IsValid(C_INSPlayer* Player)
{
	return true;
}

void BasePlayerIterator::Advance()
{
	int Entities = g_Pointers->EntityList->GetHighestEntityIndex();

	while (Index < Entities)
	{
		C_BaseEntity* Entity = static_cast<C_BaseEntity*>(g_Pointers->EntityList->GetClientEntity(Index));

		if (!Entity || strcmp(Entity->GetClassName(), "C_INSPlayer") != 0)
		{
			Index++;
			continue;
		}

		if (!this->IsValid(static_cast<C_INSPlayer*>(Entity)))
		{
			Index++;
			continue;
		}

		break;
	}
}

bool TargetPlayerIterator::IsValid(C_INSPlayer* Player)
{
	C_INSPlayer* LocalPlayer = Helpers::GetLocalPlayer();

	// TODO: The LocalPlayer checks don't work for some reason :/
	// Don't ESP yourself!
	if (Player == LocalPlayer) return false;
	if (*Player->EntIndex() == *LocalPlayer->EntIndex()) return false;
	if (*Player->GetHealth() <= 0) return false;
	if (*Player->IsDormant()) return false;
	if (*Player->GetTeam() == *LocalPlayer->GetTeam()) return false;

	return true;
}

C_BaseEntity* Helpers::GetLocalPlayerEntity()
{
	return static_cast<C_BaseEntity*>(g_Pointers->EntityList->GetClientEntity(g_Pointers->Client->GetLocalPlayerIndex()));
}

C_INSPlayer* Helpers::GetLocalPlayer()
{
	return static_cast<C_INSPlayer*>(g_Pointers->EntityList->GetClientEntity(g_Pointers->Client->GetLocalPlayerIndex()));
}
