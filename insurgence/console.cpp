#include "console.h"

#include <Windows.h>

void Console::Create()
{
	if (AllocConsole())
	{
		freopen_s(&this->Stream, "CONOUT$", "w", stdout);
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
