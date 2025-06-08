#include "console.h"

#include <Windows.h>

void Console::Style()
{
	// ANSI colors
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	if (hStdOut != INVALID_HANDLE_VALUE)
	{
		DWORD ConsoleMode;
		GetConsoleMode(hStdOut, &ConsoleMode);
		SetConsoleMode(hStdOut, ConsoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	}

	// Maximize and close buttons
	HWND Console = GetConsoleWindow();
	if (!Console) return;

	HMENU ConsoleMenu = GetSystemMenu(Console, FALSE);
	if (!ConsoleMenu) return;

	LONG lStyle = GetWindowLong(Console, GWL_STYLE);
	lStyle &= ~WS_MAXIMIZEBOX;

	SetWindowLong(Console, GWL_STYLE, lStyle);

	EnableMenuItem(ConsoleMenu, SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	EnableMenuItem(ConsoleMenu, SC_MAXIMIZE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

	DrawMenuBar(Console);
}

void Console::Create()
{
	if (AllocConsole())
	{
		freopen_s(&this->Stream, "CONOUT$", "w", stdout);

		this->Style();
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
