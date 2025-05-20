#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "globals.h"
#include "memory.h"

void UnMain(HINSTANCE Instance)
{
	if (Globals)
	{
		Globals->ExternalConsole->Destroy();
		Globals->HooksManager->Destroy();

		delete Globals;
	}

	FreeLibraryAndExitThread(Instance, 0);
}

void Main(HINSTANCE Instance)
{
	Globals = new Insurgence();

	if (!Globals->HooksManager->Create())
		return UnMain(Instance);

	Globals->ExternalConsole->Create();

	/*uintptr_t daad = Globals->MemoryManager->FindSignature("engine.dll", "48 8B 05 ? ? ? ? 48 83 C0");

	void* dafs = reinterpret_cast<void*(__cdecl*)()>(daad)();*/

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
