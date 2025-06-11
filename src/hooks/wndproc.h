#pragma once

#include "../hooks.h"

class WndProc : public Hook
{
public:
	void Create();
	void Destroy();
};
