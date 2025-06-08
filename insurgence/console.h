#pragma once

#include <iostream>

class Console
{
private:
	FILE* Stream;

public:
	void Style();

	void Create();
	void Destroy();
};
