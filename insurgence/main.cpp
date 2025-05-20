#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "globals.h"
#include "minhook/MinHook.h"

typedef bool (*fnCreateMove)(void* _this, float SampleTime, void* CUserCmd);
fnCreateMove oCreateMove;

bool __fastcall hkCreateMove(void* _this, float SampleTime, void* CUserCmd)
{
	printf("%f\n", SampleTime);
	return oCreateMove(_this, SampleTime, CUserCmd);
}

void UnMain(HINSTANCE Instance)
{
	if (Globals)
		Globals->ExternalConsole->Destroy();

	FreeLibraryAndExitThread(Instance, 0);
}

void Main(HINSTANCE Instance)
{
	if (MH_Initialize() != MH_OK)
		return UnMain(Instance);

	Globals = new Insurgence();
	Globals->ExternalConsole->Create(); // TODO: Organize globals better

	uintptr_t CreateMoveAddr = Globals->MemoryManager->FindSignature("client.dll", "40 53 48 83 EC ? 83 C9 ? 0F 29 74 24");

	MH_CreateHook((LPVOID)CreateMoveAddr, (LPVOID)&hkCreateMove, (LPVOID*)&oCreateMove);
	MH_EnableHook(MH_ALL_HOOKS);

	while (true)
	{


		Sleep(10);
	}

	MH_DisableHook(MH_ALL_HOOKS);
	MH_Uninitialize();

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
