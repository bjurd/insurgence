#include "cursor.h"

#include "../features.h"
#include "../minhook/MinHook.h"
#include "../pointers.h"
#include "../valve/isurface.h"

#include "../features/menu.h"

typedef void (*fnSetCursor)(ISurface* _this, CursorCode Cursor);
fnSetCursor oSetCursor;

typedef void (*fnUnlockCursor)(ISurface* _this);
fnUnlockCursor oUnlockCursor;

typedef void (*fnLockCursor)(ISurface* _this);
fnLockCursor oLockCursor;

static void __fastcall hkSetCursor(ISurface* _this, CursorCode Cursor)
{
	static Menu* MenuFeature = g_Features->Get<Menu>();

	if (MenuFeature && MenuFeature->IsOpen)
	{
		if (Cursor != CursorCode::dc_alwaysvisible_push && Cursor != CursorCode::dc_alwaysvisible_pop)
			Cursor = CursorCode::dc_arrow;
	}

	oSetCursor(_this, Cursor);
}

static void __fastcall hkUnlockCursor(ISurface* _this)
{
	oUnlockCursor(_this);
}

static void __fastcall hkLockCursor(ISurface* _this)
{
	static Menu* MenuFeature = g_Features->Get<Menu>();

	if (MenuFeature && MenuFeature->IsOpen)
	{
		_this->UnlockCursor();
		return;
	}

	oLockCursor(_this);
}

void Cursor::Create()
{
	MH_CreateHook((LPVOID)VMT::GetMethodPointerAt(reinterpret_cast<void***>(g_Pointers->Surface), 57), &hkSetCursor, reinterpret_cast<LPVOID*>(&oSetCursor));
	MH_CreateHook((LPVOID)VMT::GetMethodPointerAt(reinterpret_cast<void***>(g_Pointers->Surface), 66), &hkUnlockCursor, reinterpret_cast<LPVOID*>(&oUnlockCursor));
	MH_CreateHook((LPVOID)VMT::GetMethodPointerAt(reinterpret_cast<void***>(g_Pointers->Surface), 67), &hkLockCursor, reinterpret_cast<LPVOID*>(&oLockCursor));
}

void Cursor::Destroy()
{
}
