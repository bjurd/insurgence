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

bool ESP::GetPlayerBounds(C_INSPlayer* Player, float& Left, float& Right, float& Top, float& Bottom)
{
	ICollideable* Collideable = Player->GetCollideable();

	if (!Collideable)
		return false;

	Left = FLT_MAX;
	Right = -FLT_MAX;
	Top = FLT_MAX;
	Bottom = -FLT_MAX;

	const Vector Mins = Collideable->OBBMins() + Player->GetAbsOrigin();
	const Vector Maxs = Collideable->OBBMaxs() + Player->GetAbsOrigin();

	if (!Globals->PointersManager->Client->IsBoxInViewCluster(Mins, Maxs)) return false;
	if (Globals->PointersManager->Client->CullBox(Mins, Maxs)) return false; // Redundant I think?

	const Vector Corners[8] = {
		Mins,
		{ Mins.x, Maxs.y, Mins.z },
		{ Maxs.x, Maxs.y, Mins.z },
		{ Maxs.x, Mins.y, Mins.z },
		Maxs,
		{ Mins.x, Maxs.y, Maxs.z },
		{ Mins.x, Mins.y, Maxs.z },
		{ Maxs.x, Mins.y, Maxs.z }
	};

	for (int i = 0; i < 8; ++i)
	{
		Vector Temp;
		
		if (!ScreenTransform(Corners[i], Temp))
			return false;

		Left = fminf(Left, Temp.x);
		Right = fmaxf(Right, Temp.x);
		Top = fminf(Top, Temp.y);
		Bottom = fmaxf(Bottom, Temp.y);
	}

	return true;
}

void ESP::Render(LPDIRECT3DDEVICE9 Device)
{
	if (!Globals->PointersManager->Client->IsInGame())
		return;

	C_INSPlayer* LocalPlayer = Helpers::GetLocalPlayer();
	int LocalTeam = *LocalPlayer->GetTeam();

	float Left, Right, Top, Bottom;

	for (C_INSPlayer* Player : Helpers::PlayerIterator())
	{
		if (Player == LocalPlayer) continue;
		if (*Player->GetHealth() <= 0) continue;
		if (*Player->GetTeam() == LocalTeam) continue;

		if (this->GetPlayerBounds(Player, Left, Right, Top, Bottom))
		{
			float Width = Right - Left;
			float Height = Bottom - Top;

			this->DrawOutlinedRect(Device, Left, Top, Width, Height, Color(255, 0, 0, 255));
			this->DrawTextAt(Device, Player->GetPlayerName(), (int)Left, (int)Top, COLOR_WHITE);
		}
	}
}