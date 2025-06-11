#include "renderview.h"

#include "../cache.h"
#include "../minhook/MinHook.h"
#include "../signatures.h"
#include "../valve/cdll.h"
#include "../valve/view_shared.h"
#include "../pointers.h"

typedef void (*fnRenderView)(CViewRender* _this, const CViewSetup& View, const CViewSetup& OtherView, int ClearFlags, int Draw);
fnRenderView oRenderView;

static void __fastcall hkRenderView(CViewRender* _this, const CViewSetup& View, const CViewSetup& OtherView, int ClearFlags, int Draw)
{
	oRenderView(_this, View, OtherView, ClearFlags, Draw);

	CViewSetup& ViewSetup = const_cast<CViewSetup&>(View);
	ViewSetup_t& CacheViewSetup = Cache::ViewSetup;

	CacheViewSetup.X = *ViewSetup.GetX();
	CacheViewSetup.Y = *ViewSetup.GetY();
	//CacheViewSetup.Width = *ViewSetup.GetWidth(); // Bad for some reason, haven't looked into it
	//CacheViewSetup.Height = *ViewSetup.GetHeight();
	CacheViewSetup.Origin = *ViewSetup.GetOrigin();
	CacheViewSetup.Angles = *ViewSetup.GetAngles();
	CacheViewSetup.FOV = *ViewSetup.GetFOV();
}

void RenderView::Create()
{
	// TODO: Use sig
	MH_CreateHook((LPVOID)VMT::GetMethodPointerAt(reinterpret_cast<void***>(g_Pointers->ViewRender), 6), &hkRenderView, reinterpret_cast<LPVOID*>(&oRenderView));
}

void RenderView::Destroy()
{
}
