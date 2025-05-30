#include <Windows.h>

#include "globals.h"
#include "memory.h"
#include "netvars.h"

void UnMain(HINSTANCE Instance)
{
	if (Globals)
	{
		Globals->HooksManager->Destroy();
		Globals->FeaturesManager->Destroy();
		Globals->PointersManager->Destroy();
		Globals->ExternalConsole->Destroy();

		NetVars::Destroy();

		delete Globals;
	}

	FreeLibraryAndExitThread(Instance, 0);
}

void Main(HINSTANCE Instance)
{
	Globals = new Insurgence();

	Globals->ExternalConsole->Create();

	if (!Globals->PointersManager->Create())
		return UnMain(Instance);

	NetVars::Create();

	if (!Globals->FeaturesManager->Create())
		return UnMain(Instance);

	if (!Globals->HooksManager->Create())
		return UnMain(Instance);

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
		DisableThreadLibraryCalls(Instance);

		HANDLE Thread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Main, Instance, NULL, NULL);

		if (Thread)
			CloseHandle(Thread);

		break;
	}

	return TRUE;
}
