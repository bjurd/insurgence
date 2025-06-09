#include "esp.h"

#include "../cache.h"
#include "../features.h"
#include "../helpers.h"
#include "../nwi/c_insplayer.h"
#include "../pointers.h"
#include "../signatures.h"
#include "../valve/icollideable.h"
#include "../valve/math.h"
#include "../valve/render.h"
#include "../valve/studio.h"

#include "aimbot.h" // TODO: Split this away from ESP

void ESP::Create()
{
	this->Enabled = true;
	this->Boxes = true;
	this->Names = true;
}

void ESP::Destroy()
{
}

void ESP::SetupFont(LPDIRECT3DDEVICE9 Device)
{
	// TODO: Detect failure
	D3DXCreateFont(Device, 24, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"), &this->Font);
}

void ESP::DrawTextAt(LPDIRECT3DDEVICE9 Device, const std::string Text, const int X, const int Y, const Color TextColor)
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

void ESP::DrawOutlinedRect(LPDIRECT3DDEVICE9 Device, const float X, const float Y, const float Width, const float Height, const Color InnerColor)
{
	// Would probably be faster to copy the code above and edit the points instead of calling this 3 times
	// Oh well, this looks nicer :^)

	this->DrawRect(Device, X, Y, Width, Height, InnerColor);
	this->DrawRect(Device, X - 1, Y - 1, Width + 2, Height + 2, Colors::Black);
	this->DrawRect(Device, X + 1, Y + 1, Width - 2, Height - 2, Colors::Black);
}

void ESP::DrawCircle(LPDIRECT3DDEVICE9 Device, const float X, const float Y, const float Radius, const Color OutlineColor)
{
	static int Segments = 32;

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
			Line->Draw(Points.data(), (UINT)Points.size(), D3DCOLOR_ARGB(OutlineColor.a, OutlineColor.r, OutlineColor.g, OutlineColor.b));
		}
		Line->End();

		Line->Release();
	}
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

	if (!g_Pointers->EngineClient->IsBoxInViewCluster(Mins, Maxs)) return false;
	if (g_Pointers->EngineClient->CullBox(Mins, Maxs)) return false; // Redundant I think?

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

void ESP::SetupRenderStateBlock(LPDIRECT3DDEVICE9 Device)
{
	if (this->RenderStateBlock)
		this->DestroyStateBlocks();

	Device->BeginStateBlock();
	{
		this->SetupRenderState(Device);
	}
	Device->EndStateBlock(&this->RenderStateBlock);
}

void ESP::DestroyStateBlocks()
{
	if (this->RenderStateBlock)
	{
		this->RenderStateBlock->Release();
		this->RenderStateBlock = nullptr;
	}

	if (this->GameStateBlock)
	{
		this->GameStateBlock->Release();
		this->GameStateBlock = nullptr;
	}
}

void ESP::SetupRenderState(LPDIRECT3DDEVICE9 Device)
{
	int ScreenWidth = Cache::ViewSetup.Width;
	int ScreenHeight = Cache::ViewSetup.Height;

	D3DVIEWPORT9 ViewPort;
	ViewPort.X = ViewPort.Y = 0;
	ViewPort.Width = (DWORD)ScreenWidth;
	ViewPort.Height = (DWORD)ScreenHeight;
	ViewPort.MinZ = 0.f;
	ViewPort.MaxZ = 1.f;

	Device->SetViewport(&ViewPort);

	Device->SetPixelShader(nullptr); // (Mostly) Stolen from ImGui
	Device->SetVertexShader(nullptr);
	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	Device->SetRenderState(D3DRS_ZENABLE, FALSE);
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	Device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	Device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
	Device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA);
	Device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
	Device->SetRenderState(D3DRS_FOGENABLE, FALSE);
	Device->SetRenderState(D3DRS_RANGEFOGENABLE, FALSE);
	Device->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
	Device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	Device->SetRenderState(D3DRS_CLIPPING, TRUE);
	Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	Device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	D3DXMATRIX Identity;
	D3DXMatrixIdentity(&Identity);

	Device->SetTransform(D3DTS_WORLD, &Identity);
	Device->SetTransform(D3DTS_VIEW, &Identity);

	D3DXMATRIX Ortho;
	D3DXMatrixOrthoOffCenterLH(&Ortho, 0.f, (float)ScreenWidth, (float)ScreenHeight, 0.f, 0.f, 1.f);

	Device->SetTransform(D3DTS_PROJECTION, &Ortho);
}

void ESP::Render(LPDIRECT3DDEVICE9 Device)
{
	if (!this->Enabled || !g_Pointers->EngineClient->IsInGame())
		return;

	if (!this->GameStateBlock)
	{
		if (FAILED(Device->CreateStateBlock(D3DSBT_ALL, &this->GameStateBlock)))
			return;
	}

	if (!this->RenderStateBlock)
		this->SetupRenderStateBlock(Device);

	if (FAILED(this->GameStateBlock->Capture()))
		return;

	if (FAILED(this->RenderStateBlock->Apply()))
		return;

	this->InScene = true;
	Device->BeginScene();
	{
		float Left, Right, Top, Bottom;

		for (C_INSPlayer* Player : Helpers::Iterators::TargetPlayers())
		{
			if (!Player) continue;

			if (this->GetPlayerBounds(Player, Left, Right, Top, Bottom))
			{
				float Width = Right - Left;
				float Height = Bottom - Top;

				if (this->Boxes) this->DrawOutlinedRect(Device, Left, Top, Width, Height, Colors::Red);
				if (this->Names) this->DrawTextAt(Device, Player->GetPlayerName(), static_cast<int>(Left), static_cast<int>(Top), Colors::White);
			}
		}

		static Aimbot* AimbotFeature = g_Features->Get<Aimbot>("Aimbot");

		if (AimbotFeature && AimbotFeature->Enabled)
		{
			float AimbotRadius = AimbotFeature->GetFOVRadius();

			int ScreenWidth = Cache::ViewSetup.Width;
			int ScreenHeight = Cache::ViewSetup.Height;

			this->DrawCircle(Device, static_cast<float>(ScreenWidth / 2), static_cast<float>(ScreenHeight / 2), AimbotRadius - 1, Colors::Black);
			this->DrawCircle(Device, static_cast<float>(ScreenWidth / 2), static_cast<float>(ScreenHeight / 2), AimbotRadius + 1, Colors::Black);
			this->DrawCircle(Device, static_cast<float>(ScreenWidth / 2), static_cast<float>(ScreenHeight / 2), AimbotRadius, Colors::White);
		}
	}
	Device->EndScene();
	this->InScene = false;

	this->GameStateBlock->Apply();
}
