#pragma once

#include "valve/color.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <utility>

#undef DrawText // Fuck u

namespace Draw
{
	inline ID3DXLine* Line;
	inline ID3DXFont* Font;

	bool SetupLine(LPDIRECT3DDEVICE9 Device);
	bool SetupFont(LPDIRECT3DDEVICE9 Device);

	std::pair<int, int> GetTextSize(LPDIRECT3DDEVICE9 Device, const std::string Text);
	void DrawText(LPDIRECT3DDEVICE9 Device, const std::string Text, const int X, const int Y, const Color TextColor);

	void DrawRect(LPDIRECT3DDEVICE9 Device, const float X, const float Y, const float Width, const float Height, const Color InnerColor);
	void DrawOutlinedRect(LPDIRECT3DDEVICE9 Device, const float X, const float Y, const float Width, const float Height, const Color InnerColor);
	void DrawFilledRect(LPDIRECT3DDEVICE9 Device, const float X, const float Y, const float Width, const float Height, const Color InnerColor);

	void DrawCircle(LPDIRECT3DDEVICE9 Device, const float X, const float Y, const float Radius, const Color InnerColor, const int Segments);
	void DrawOutlinedCircle(LPDIRECT3DDEVICE9 Device, const float X, const float Y, const float Radius, const Color InnerColor, const int Segments);

	void Destroy();
}
