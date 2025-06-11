#pragma once

#include "../signatures.h"
#include "../vmt.h"

class CViewSetup;

class IViewRender
{
public:
};

class CViewRender : public IViewRender
{
public:
	CLPROXY(RenderView, CViewRender_RenderView, void, (const CViewSetup& View, const CViewSetup& OtherView, int ClearFlags, int Draw), VWRAP(View), VWRAP(OtherView), ClearFlags, Draw);
	CLPROXY(GetPlayerViewSetup, CViewRender_GetPlayerViewSetup, const CViewSetup*, (int));
};
