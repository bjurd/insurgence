#pragma once

#include "valve/icliententitylist.h"
#include "valve/ivengineclient.h"
#include "valve/client.h"

class Pointers
{
public:
	IClientEntityList* EntityList;
	IVEngineClient* Client;
	CClientState* ClientState;

public:
	void Create();
	void Destroy();
};