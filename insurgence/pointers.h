#pragma once

#include "valve/client.h"
#include "valve/enginetrace.h"
#include "valve/icliententitylist.h"
#include "valve/ienginevgui.h"
#include "valve/isurface.h"
#include "valve/ivengineclient.h"
#include "valve/ivmodelinfo.h"

#include <cinttypes>
#include <cstdio>
#include <iostream>

class Pointers
{
public:
	IClientEntityList* EntityList;
	IVEngineClient* Client;
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

	template <typename T>
	bool PrintGrabInterface(const std::string Name, const std::string Module, const std::string Interface, T*& Output)
	{
		if (this->TryGrabInterface<T>(Module, Interface, Output))
		{
			// std::cout << Name << ": 0x" << std::setw(16) << std::setfill('0') << std::hex << std::uppercase << reinterpret_cast<uintptr_t>(Output) << std::nouppercase << std::dec << std::setfill(' ') << std::endl;
			printf(
				"%s: 0x%016" PRIXPTR "\n",
				Name.c_str(),
				reinterpret_cast<uintptr_t>(Output)
			);

			return true;
		}

		std::cout << "Failed to get pointer for " << Name << std::endl;

		return false;
	}

	bool Create();
	void Destroy();
};

inline Pointers* g_Pointers;
