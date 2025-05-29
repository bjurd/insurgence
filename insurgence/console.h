#pragma once

#include <iostream>

class Console
{
private:
	FILE* Stream;

public:
	void Create();
	void Destroy();
};
