#include <Windows.h>

#include "binds.h"
#include "console.h"
#include "features.h"
#include "hooks.h"
#include "memory.h"
#include "netvars.h"
#include "pointers.h"

#define CreateLib(Name) g_##Name = new Name(); if (!g_##Name || !g_##Name->Create()) return UnMain(Instance);
#define FreeLibPtr(Lib) if (Lib) { Lib->Destroy(); } FreePtr(Lib);

void UnMain(HINSTANCE Instance)
{
	Binds::Destroy();

	FreeLibPtr(g_Hooks);
	FreeLibPtr(g_Features);
	FreeLibPtr(g_Pointers);
	FreeLibPtr(g_Console);

	NetVars::Destroy();

	FreeLibraryAndExitThread(Instance, 0);
}

void Main(HINSTANCE Instance)
{
	CreateLib(Console);
	CreateLib(Pointers);

	NetVars::Create();

	CreateLib(Features);
	CreateLib(Hooks);

	while (true)
	{
		if (GetAsyncKeyState(VK_END))
			break;

		Sleep(10);
	}

	UnMain(Instance);
}

BOOL WINAPI DllMain(HINSTANCE Instance, DWORD Reason, LPVOID Reserved)
{
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH:
	{
		HANDLE Thread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Main, Instance, NULL, NULL);

		if (!Thread)
			return FALSE;

		CloseHandle(Thread);

		break;
	}

	case DLL_PROCESS_DETACH: // TODO
		break;
	}

	return TRUE;
}
