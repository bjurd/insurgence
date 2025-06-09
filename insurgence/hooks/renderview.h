#pragma once

#include "../hooks.h"

class RenderView : public Hook
{
public:
	void Create();
	void Destroy();
};
