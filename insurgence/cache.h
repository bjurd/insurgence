#pragma once

#include "valve/matrix.h"
#include "valve/view_shared.h"

namespace Cache
{
	inline VMatrix WorldToScreenMatrix;
	inline CViewSetup ViewSetup; // Updated in RenderView hook

	void FrameUpdate();
}
