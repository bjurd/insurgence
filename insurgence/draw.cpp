#include "draw.h"

#include <vector>

void Draw::SetupFont(LPDIRECT3DDEVICE9 Device)
{
	// TODO: Detect failure
	D3DXCreateFont(Device, 24, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"), &Draw::Font);
}

void Draw::DrawText(LPDIRECT3DDEVICE9 Device, const std::string Text, const int X, const int Y, const Color TextColor)
{
	if (!Draw::Font)
		Draw::SetupFont(Device);

	RECT Clip = { X, Y, 0, 0 };

	Draw::Font->DrawTextA(nullptr, Text.c_str(), -1, &Clip, DT_NOCLIP, D3DCOLOR_ARGB(TextColor.a, TextColor.r, TextColor.g, TextColor.b));
}

void Draw::DrawRect(LPDIRECT3DDEVICE9 Device, const float X, const float Y, const float Width, const float Height, const Color InnerColor)
{
	ID3DXLine* Line = nullptr;

	if (SUCCEEDED(D3DXCreateLine(Device, &Line)))
	{
		D3DXVECTOR2 Points[5];
		Points[0] = D3DXVECTOR2(X, Y);
		Points[1] = D3DXVECTOR2(X + Width, Y);
		Points[2] = D3DXVECTOR2(X + Width, Y + Height);
		Points[3] = D3DXVECTOR2(X, Y + Height);
		Points[4] = Points[0];

		Line->SetWidth(1.f);
		Line->SetAntialias(FALSE);

		Line->Begin();
		{
			Line->Draw(Points, 5, D3DCOLOR_ARGB(InnerColor.a, InnerColor.r, InnerColor.g, InnerColor.b));
		}
		Line->End();

		Line->Release();
	}
}

void Draw::DrawOutlinedRect(LPDIRECT3DDEVICE9 Device, const float X, const float Y, const float Width, const float Height, const Color InnerColor)
{
	// Would probably be faster to copy the code above and edit the points instead of calling this 3 times
	// Oh well, this looks nicer :^)

	Draw::DrawRect(Device, X, Y, Width, Height, InnerColor);
	Draw::DrawRect(Device, X - 1, Y - 1, Width + 2, Height + 2, Colors::Black);
	Draw::DrawRect(Device, X + 1, Y + 1, Width - 2, Height - 2, Colors::Black);
}

void Draw::DrawCircle(LPDIRECT3DDEVICE9 Device, const float X, const float Y, const float Radius, const Color InnerColor, const int Segments)
{
	ID3DXLine* Line = nullptr;

	if (SUCCEEDED(D3DXCreateLine(Device, &Line)))
	{
		std::vector<D3DXVECTOR2> Points(Segments + 1);
		float Step = D3DX_PI * 2.f / Segments;

		for (int i = 0; i <= Segments; ++i)
		{
			float Angle = i * Step;

			Points[i] = D3DXVECTOR2(
				X + cosf(Angle) * Radius,
				Y + sinf(Angle) * Radius
			);
		}

		Line->SetWidth(1.f);
		Line->SetAntialias(FALSE);

		Line->Begin();
		{
			Line->Draw(Points.data(), (UINT)Points.size(), D3DCOLOR_ARGB(InnerColor.a, InnerColor.r, InnerColor.g, InnerColor.b));
		}
		Line->End();

		Line->Release();
	}
}

void Draw::DrawOutlinedCircle(LPDIRECT3DDEVICE9 Device, const float X, const float Y, const float Radius, const Color InnerColor, const int Segments)
{
	Draw::DrawCircle(Device, X, Y, Radius - 1, Colors::Black, Segments);
	Draw::DrawCircle(Device, X, Y, Radius + 1, Colors::Black, Segments);
	Draw::DrawCircle(Device, X, Y, Radius, InnerColor, Segments);
}

void Draw::Destroy()
{
	if (Draw::Font)
	{
		Draw::Font->Release();
		Draw::Font = nullptr;
	}
}
