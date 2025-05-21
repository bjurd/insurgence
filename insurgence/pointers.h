#pragma once

#include "valve/icliententitylist.h"
#include "valve/ivengineclient.h"
#include "valve/client.h"

class Pointers
{
public:
	bool Initialized;

	IClientEntityList* EntityList;
	IVEngineClient* Client;
	CClientState* ClientState;

public:
	void Create();
	void Destroy();
};