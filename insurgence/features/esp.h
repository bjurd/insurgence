#pragma once

#include "../features.h"
#include "../valve/color.h"
#include "../nwi/c_insplayer.h"
#include <d3d9.h>
#include <d3dx9.h>

class ESP : public Feature
{
public:
	void Create();
	void Destroy();

public:
	// ESP serving as main rendering class? Such organization!!!
	// I'll change it at some point (probably)
	ID3DXFont* Font;

	void SetupFont(LPDIRECT3DDEVICE9 Device); // This can't be done in Create because LPDIRECT3DDEVICE9 is needed
	void DrawTextAt(LPDIRECT3DDEVICE9 Device, std::string Text, const int X, const int Y, const Color TextColor); // WinUser.h has DrawText define :(

	void DrawRect(LPDIRECT3DDEVICE9 Device, const float X, const float Y, const float Width, const float Height, const Color InnerColor);
	void DrawOutlinedRect(LPDIRECT3DDEVICE9 Device, const float X, const float Y, const float Width, const float Height, const Color InnerColor);

	bool GetPlayerBounds(C_INSPlayer* Player, float& Left, float& Top, float& Right, float& Bottom);

	void Render(LPDIRECT3DDEVICE9 Device);
};
