#pragma once

#include "../features.h"
#include <d3d9.h>

class Menu : public Feature
{
public:
	bool IsSetup;
	bool IsOpen;

public:
	void Create();
	void Destroy();

public:
	void Setup(LPDIRECT3DDEVICE9 Device);
	void Render();
};
