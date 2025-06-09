#include "paint.h"

#include "../cache.h"
#include "../minhook/MinHook.h"
#include "../pointers.h"
#include "../valve/ienginevgui.h"
#include "../vmt.h"

typedef void (*fnPaint)(IEngineVGuiInternal* _this, PaintMode_t Mode);
fnPaint oPaint;

void __fastcall hkPaint(IEngineVGuiInternal* _this, PaintMode_t Mode)
{
	if (Mode & PAINT_UIPANELS)
	{
		Cache::FrameUpdate();
	}

	oPaint(_this, Mode);
}

void Paint::Create()
{
	MH_CreateHook((LPVOID)VMT::GetMethodPointerAt(reinterpret_cast<void***>(g_Pointers->EngineVGui), 14), &hkPaint, reinterpret_cast<LPVOID*>(&oPaint));
}

void Paint::Destroy()
{
}
