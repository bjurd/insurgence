#include "cache.h"

#include "pointers.h"

void Cache::FrameUpdate()
{
	Cache::WorldToScreenMatrix = g_Pointers->Client->WorldToScreenMatrix();
}
