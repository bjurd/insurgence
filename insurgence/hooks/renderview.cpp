#include "renderview.h"

#include "../cache.h"
#include "../minhook/MinHook.h"
#include "../signatures.h"
#include "../valve/cdll.h"
#include "../valve/view_shared.h"
#include "../pointers.h"

typedef void (*fnRenderView)(CViewRender* _this, const CViewSetup& View, const CViewSetup& OtherView, int ClearFlags, int Draw);
fnRenderView oRenderView;

void __fastcall hkRenderView(CViewRender* _this, const CViewSetup& View, const CViewSetup& OtherView, int ClearFlags, int Draw)
{
	oRenderView(_this, View, OtherView, ClearFlags, Draw);

	CViewSetup& ViewSetup = const_cast<CViewSetup&>(View);

	Cache::ViewSetup.X = *ViewSetup.GetX();
	Cache::ViewSetup.Y = *ViewSetup.GetY();
	Cache::ViewSetup.Width = *ViewSetup.GetWidth();
	Cache::ViewSetup.Height = *ViewSetup.GetHeight();
	Cache::ViewSetup.FOV = *ViewSetup.GetFOV();
}

void RenderView::Create()
{
	// TODO: Use sig
	MH_CreateHook((LPVOID)VMT::GetMethodPointerAt(reinterpret_cast<char***>(g_Pointers->ViewRender), 6), &hkRenderView, reinterpret_cast<LPVOID*>(&oRenderView));
}

void RenderView::Destroy()
{
}
