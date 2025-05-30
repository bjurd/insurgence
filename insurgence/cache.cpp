#include "cache.h"

#include "globals.h"

void Cache::FrameUpdate()
{
	Cache::WorldToScreenMatrix = Globals->PointersManager->Client->WorldToScreenMatrix();
}
