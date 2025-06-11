#pragma once

#include <iostream>

class Console
{
private:
	FILE* Stream;

public:
	void Style();

	bool Create();
	void Destroy();
};

inline Console* g_Console;
