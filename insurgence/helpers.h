#pragma once

#include "valve/c_baseentity.h"
#include "nwi/c_insplayer.h"
#include "globals.h"

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

class Helpers
{
public:
	static C_BaseEntity* GetLocalPlayerEntity();
	static C_INSPlayer* GetLocalPlayer();

    static inline PlayerRange PlayerIterator()
    {
        return PlayerRange();
    }
};