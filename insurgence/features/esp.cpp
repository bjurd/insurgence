#include "esp.h"

#include "../cache.h"
#include "../draw.h"
#include "../features.h"
#include "../helpers.h"
#include "../nwi/c_insplayer.h"
#include "../pointers.h"
#include "../signatures.h"
#include "../valve/icollideable.h"
#include "../valve/math.h"
#include "../valve/render.h"
#include "../valve/studio.h"
#include <algorithm>

#include "aimbot.h" // TODO: Split this away from ESP

void ESP::Create()
{
}

void ESP::Destroy()
{
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
				const float Width = Right - Left;
				const float Height = Bottom - Top;

				if (this->Boxes)
					Draw::DrawOutlinedRect(Device, Left, Top, Width, Height, Colors::Red);

				if (this->Health)
				{
					float HealthPercentage = static_cast<float>(*Player->GetHealth()) / static_cast<float>(*Player->GetMaxHealth());
					HealthPercentage = std::clamp<float>(HealthPercentage, 0.f, 1.f);

					const float HealthHeight = (Height - 2.f) * HealthPercentage;

					Draw::DrawFilledRect(Device, Left - 5.f, Top, 4.f, Height, Colors::Black);
					Draw::DrawFilledRect(Device, Left - 4.f, (Bottom - HealthHeight) - 1.f, 2.f, HealthHeight, Colors::Green);
				}

				if (this->Names)
				{
					std::pair<int, int> TextSize = Draw::GetTextSize(Device, Player->GetPlayerName());

					float Center = Left + (Width / 2.f);
					float TextX = Center - (TextSize.first / 2.f);
					float TextY = Top - TextSize.second;

					Draw::DrawText(Device, Player->GetPlayerName(), static_cast<int>(TextX), static_cast<int>(TextY), Colors::White);
				}
			}
		}

		static Aimbot* AimbotFeature = g_Features->Get<Aimbot>("Aimbot");

		if (AimbotFeature && AimbotFeature->Enabled)
		{
			float AimbotRadius = AimbotFeature->GetFOVRadius();

			Draw::DrawOutlinedCircle(Device, static_cast<float>(Cache::ViewSetup.Width / 2), static_cast<float>(Cache::ViewSetup.Height / 2), AimbotRadius, Colors::White, 64);
		}
	}
	Device->EndScene();
	this->InScene = false;

	this->GameStateBlock->Apply();
}
