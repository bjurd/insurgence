#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "globals.h"

void Main(HINSTANCE Instance)
{
	Globals = new Insurgence();

	if (Globals->MemoryManager->FindSignature("client.dll", "40 53 48 83 EC ? 83 C9 ? 0F 29 74 24"))
	{
		MessageBoxA(NULL, "yay", "yay", MB_OK);
	}
	else
	{
		MessageBoxA(NULL, "no yay", "no yay", MB_OK);
	}

	while (true)
	{


		Sleep(10);
	}

	FreeLibraryAndExitThread(Instance, 0);
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