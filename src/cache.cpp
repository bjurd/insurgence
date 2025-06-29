#include "cache.h"

#include "pointers.h"
#include "memory.h"
#include "signatures.h"

void Cache::FrameUpdate()
{
	Cache::WorldToScreenMatrix = g_Pointers->EngineClient->WorldToScreenMatrix();

	g_Pointers->EngineClient->GetScreenSize(Cache::ViewSetup.Width, Cache::ViewSetup.Height);
}
