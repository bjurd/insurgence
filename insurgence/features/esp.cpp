#include "esp.h"

#include "../globals.h"
#include "../helpers.h"
#include "../nwi/c_insplayer.h"
#include "../signatures.h"
#include "../valve/math.h"
#include "../valve/render.h"
#include "../valve/studio.h"

void ESP::Create()
{
	
}

void ESP::Destroy()
{

}

void ESP::SetupFont(LPDIRECT3DDEVICE9 Device)
{
	// TODO: Detect failure
	D3DXCreateFont(Device, 24, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"), &this->Font);
}

void ESP::DrawTextAt(LPDIRECT3DDEVICE9 Device, std::string Text, const int X, const int Y, const Color TextColor)
{
	if (!this->Font)
		this->SetupFont(Device);

	RECT Clip = { X, Y, 0, 0 };

	this->Font->DrawTextA(nullptr, Text.c_str(), -1, &Clip, DT_NOCLIP, D3DCOLOR_ARGB(TextColor.a, TextColor.r, TextColor.g, TextColor.b));
}

void ESP::DrawRect(LPDIRECT3DDEVICE9 Device, const float X, const float Y, const float Width, const float Height, const Color InnerColor)
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

		Line->SetWidth(1.0f);
		Line->SetAntialias(FALSE);

		Line->Begin();
		{
			Line->Draw(Points, 5, D3DCOLOR_ARGB(InnerColor.a, InnerColor.r, InnerColor.g, InnerColor.b));
		}
		Line->End();

		Line->Release();
	}
}

void ESP::DrawOutlinedRect(LPDIRECT3DDEVICE9 Device, const float X, const float Y, const float Width, const float Height, const Color InnerColor)
{
	// Would probably be faster to copy the code above and edit the points instead of calling this 3 times
	// Oh well, this looks nicer :^)

	this->DrawRect(Device, X, Y, Width, Height, InnerColor);
	this->DrawRect(Device, X - 1, Y - 1, Width + 2, Height + 2, COLOR_BLACK);
	this->DrawRect(Device, X + 1, Y + 1, Width - 2, Height - 2, COLOR_BLACK);
}

void ESP::Render(LPDIRECT3DDEVICE9 Device)
{
	if (!Globals->PointersManager->Client->IsInGame())
		return;

	C_INSPlayer* LocalPlayer = Helpers::GetLocalPlayer();
	Vector LocalPlayerOrigin = LocalPlayer->GetAbsOrigin();

	for (C_INSPlayer* Player : Helpers::PlayerIterator())
	{
		if (Player == LocalPlayer) continue;
		if (*Player->GetHealth() <= 0) continue;

		Vector PlayerOrigin = Player->GetAbsOrigin();
		Vector ScreenOrigin;

		if (ScreenTransform(PlayerOrigin, ScreenOrigin))
		{
			this->DrawTextAt(Device, std::to_string(*Player->GetHealth()), (int)ScreenOrigin.x, (int)ScreenOrigin.y, COLOR_WHITE);
		}
	}

	//this->DrawOutlinedRect(Device, 10, 10, 50, 150, Color(255, 0, 0, 255));
	//this->DrawTextAt(Device, "StormyStyx Sux Dyx", 5, 160, COLOR_WHITE);
}