#pragma once

#include "valve/matrix.h"
#include "valve/vector.h"

struct ViewSetup_t
{
	int X, Y;
	int Width, Height;

	Vector Origin;
	Angle Angles;

	float FOV;
};

namespace Cache
{
	inline VMatrix WorldToScreenMatrix;
	inline ViewSetup_t ViewSetup; // Updated in RenderView hook

	void FrameUpdate();
}
