#pragma once

#include "valve/matrix.h"
#include "valve/vector.h"

struct ViewSetup_t
{
	int X = 0;
	int Y = 0;
	int Width = 0;
	int Height = 0;

	Vector Origin;
	Angle Angles;

	float FOV = 0.f;
};

namespace Cache
{
	inline VMatrix WorldToScreenMatrix;
	inline ViewSetup_t ViewSetup; // Updated in RenderView hook

	void FrameUpdate();
}
