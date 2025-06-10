#pragma once

#include "../features.h"
#include "../nwi/c_insplayer.h"

class ESP : public Feature
{
public:
	bool InScene; // TODO: A better way of communicating with the DirectX hook

public: // TODO: A config system with JSON or YAML or something
	bool Enabled = true;
	bool Boxes = true;
	bool Names = true;
	bool Health = true;

private:
	IDirect3DStateBlock9* GameStateBlock, * RenderStateBlock;

public:
	void Create();
	void Destroy();

public:
	bool GetPlayerBounds(C_INSPlayer* Player, float& Left, float& Top, float& Right, float& Bottom);

	bool SetupRenderStateBlock(LPDIRECT3DDEVICE9 Device);
	void DestroyStateBlocks();
	bool SetupRenderState(LPDIRECT3DDEVICE9 Device);

	void Render(LPDIRECT3DDEVICE9 Device);
};
