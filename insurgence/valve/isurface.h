#pragma once

#include "iappsystem.h"
#include "cursor.h"

#include "../vmt.h"

// TODO: Sigproxy these
class ISurface : public IAppSystem
{
public:
	VPROXY(SetCursor, 57, void, (CursorCode Cursor));
	VPROXY(IsCursorVisible, 58, bool, (void));
	VPROXY(UnlockCursor, 66, void, (void));
	VPROXY(LockCursor, 67, void, (void));
};
