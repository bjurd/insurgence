#pragma once

#include "../features.h"
#include "../valve/color.h"
#include "../nwi/c_insplayer.h"
#include <d3d9.h>
#include <d3dx9.h>

class ESP : public Feature
{
public:
	bool InScene; // TODO: A better way of communicating with the DirectX hook

public: // TODO: A config system with JSON or YAML or something
	bool Enabled;
	bool Boxes;
	bool Names;

private:
	IDirect3DStateBlock9* GameStateBlock, * RenderStateBlock;

public:
	void Create();
	void Destroy();

public:
	// ESP serving as main rendering class? Such organization!!!
	// I'll change it at some point (probably)
	ID3DXFont* Font;

	void SetupFont(LPDIRECT3DDEVICE9 Device); // This can't be done in Create because LPDIRECT3DDEVICE9 is needed
	void DrawTextAt(LPDIRECT3DDEVICE9 Device, const std::string Text, const int X, const int Y, const Color TextColor); // WinUser.h has DrawText define :(

	void DrawRect(LPDIRECT3DDEVICE9 Device, const float X, const float Y, const float Width, const float Height, const Color InnerColor);
	void DrawOutlinedRect(LPDIRECT3DDEVICE9 Device, const float X, const float Y, const float Width, const float Height, const Color InnerColor);
	void DrawCircle(LPDIRECT3DDEVICE9 Device, const float X, const float Y, const float Radius, const Color InnerColor, const int Segments);
	void DrawOutlinedCircle(LPDIRECT3DDEVICE9 Device, const float X, const float Y, const float Radius, const Color InnerColor, const int Segments);

	bool GetPlayerBounds(C_INSPlayer* Player, float& Left, float& Top, float& Right, float& Bottom);

	void SetupRenderStateBlock(LPDIRECT3DDEVICE9 Device);
	void DestroyStateBlocks();
	void SetupRenderState(LPDIRECT3DDEVICE9 Device);

	void Render(LPDIRECT3DDEVICE9 Device);
};
