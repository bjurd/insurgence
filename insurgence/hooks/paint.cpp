#include "paint.h"

#include "../valve/ienginevgui.h"
#include "../cache.h"
#include "../globals.h"
#include "../minhook/MinHook.h"
#include "../vmt.h"

typedef void (*fnPaint)(IEngineVGuiInternal* _this, PaintMode_t Mode);
fnPaint oPaint;

void __stdcall hkPaint(IEngineVGuiInternal* _this, PaintMode_t Mode)
{
	oPaint(_this, Mode);

	if (Mode & PAINT_INGAMEPANELS)
		Cache::FrameUpdate();
}

void Paint::Create()
{
	MH_CreateHook((LPVOID)VMT::GetMethodPointerAt(reinterpret_cast<char***>(Globals->PointersManager->EngineVGui), 14), &hkPaint, reinterpret_cast<LPVOID*>(&oPaint));
}

void Paint::Destroy()
{
}
