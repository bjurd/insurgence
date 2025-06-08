#include "console.h"

#include <Windows.h>

void Console::Create()
{
	if (AllocConsole())
	{
		freopen_s(&this->Stream, "CONOUT$", "w", stdout);

		HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

		if (hStdOut != INVALID_HANDLE_VALUE)
		{
			DWORD ConsoleMode;
			GetConsoleMode(hStdOut, &ConsoleMode);
			SetConsoleMode(hStdOut, ConsoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
		}
	}
}

void Console::Destroy()
{
	if (this->Stream)
	{
		fclose(this->Stream);
		FreeConsole();
	}
}
