#pragma once

#include "valve/icliententitylist.h"
#include "valve/ivengineclient.h"
#include "valve/client.h"
#include "valve/ivmodelinfo.h"

class Pointers
{
public:
	IClientEntityList* EntityList;
	IVEngineClient* Client;
	CClientState* ClientState;
	IVModelInfo* ModelInfo;

public:
	void Create();
	void Destroy();
};