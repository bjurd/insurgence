#pragma once

#include "../vmt.h"
#include "matrix.h"
#include "vector.h"

enum ClearFlags_t
{
	VIEW_CLEAR_COLOR = 0x1,
	VIEW_CLEAR_DEPTH = 0x2,
	VIEW_CLEAR_FULL_TARGET = 0x4,
	VIEW_NO_DRAW = 0x8,
	VIEW_CLEAR_OBEY_STENCIL = 0x10,
	VIEW_CLEAR_STENCIL = 0x20,
};

enum StereoEye_t
{
	STEREO_EYE_MONO = 0,
	STEREO_EYE_LEFT = 1,
	STEREO_EYE_RIGHT = 2,
	STEREO_EYE_MAX = 3,
};

class CViewSetup // TODO: This class has a temporary retard setup just to get it to work and needs fixed at some point
{
public: // Grab from engine
	PROXYVAR(GetX, int, 0x0000);
	PROXYVAR(GetY, int, 0x0004);
	PROXYVAR(GetWidth, int, 0x0008);
	PROXYVAR(GetHeight, int, 0x000C);

	PROXYVAR(GetFOV, float, 0x00A8);

	PROXYVAR(GetZNear, float, 0x0200);
	PROXYVAR(GetZFar, float, 0x0204);

	PROXYVAR(GetOffCenter, bool, 0x0320);

public: // Use in insurgence
	int X, Y;
	int Width, Height;

	float FOV;
};
