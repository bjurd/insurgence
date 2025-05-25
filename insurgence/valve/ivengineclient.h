#pragma once

#include "vector.h"
#include "matrix.h"
#include "buttoncode.h"
#include "../vmt.hpp"

class IVEngineClient
{
public:
	VPROXY(GetScreenSize, 5, void, (int& X, int& Y), VWRAP(X), VWRAP(Y))
	VPROXY(GetLocalPlayerIndex, 12, int, (void))
	VPROXY(GetViewAngles, 18, void, (Angle& Angles), VWRAP(Angles))
	VPROXY(SetViewAngles, 19, void, (Angle& Angles), VWRAP(Angles))
	VPROXY(GetFreeaimAngles, 20, void, (Angle& Angles), VWRAP(Angles))
	VPROXY(SetFreeaimAngles, 21, void, (Angle& Angles), VWRAP(Angles))
	VPROXY(IsInGame, 28, bool, (void))
	VPROXY(IsConnected, 29, bool, (void))
	VPROXY(IsBoxInViewCluster, 35, int, (const Vector& Mins, const Vector& Maxs), VWRAP(Mins), VWRAP(Maxs))
	VPROXY(CullBox, 36, bool, (const Vector& Mins, const Vector& Maxs), VWRAP(Mins), VWRAP(Maxs))
	VPROXY(WorldToScreenMatrix, 39, VMatrix&, (void))
};