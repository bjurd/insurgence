#pragma once

#include "valve/cdll.h"
#include "valve/client.h"
#include "valve/enginetrace.h"
#include "valve/icliententitylist.h"
#include "valve/ienginevgui.h"
#include "valve/isurface.h"
#include "valve/ivengineclient.h"
#include "valve/ivmodelinfo.h"
#include "valve/view.h"
#include "valve/viewrender.h"

#include <cinttypes>
#include <cstdio>
#include <iostream>

class Pointers
{
public:
	IClientEntityList* EntityList;
	IVEngineClient* EngineClient;
	CHLClient* Client;
	CViewRender* ViewRender;
	CVRenderView* RenderView;
	CClientState* ClientState;
	IVModelInfo* ModelInfo;
	CEngineTrace* EngineTrace;
	IEngineVGuiInternal* EngineVGui;
	ISurface* Surface;

public:
	template <typename T>
	bool TryGrabInterface(const std::string Module, const std::string Interface, T*& Output)
	{
		return (Output = Memory::GetInterface<T*>(Module, Interface));
	}

	void PrintAddress(const std::string Name, void* Object);

	template <typename T>
	bool PrintGrabInterface(const std::string Name, const std::string Module, const std::string Interface, T*& Output)
	{
		if (this->TryGrabInterface<T>(Module, Interface, Output))
		{
			this->PrintAddress(Name, Output);

			return true;
		}

		std::cout << "Failed to get pointer for " << Name << std::endl;

		return false;
	}

	bool Create();
	void Destroy();
};

inline Pointers* g_Pointers;
