#pragma once

#include "../signatures.h"
#include "../vmt.h"

class CViewSetup;

class IBaseClientDLL
{
public:
};

class CHLClient : public IBaseClientDLL
{
public:
	CLPROXY(RenderView, CHLClient_RenderView, void, (const CViewSetup& View, int ClearFlags, int Draw), VWRAP(View), ClearFlags, Draw);
	CLPROXY(GetPlayerView, CHLClient_GetPlayerView, bool, (CViewSetup& View), VWRAP(View));
};
