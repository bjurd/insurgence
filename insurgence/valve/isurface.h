#pragma once

#include "iappsystem.h"

#include "../vmt.h"

class ISurface : public IAppSystem
{
public:
	VPROXY(UnlockCursor, 66, void, (void));
	VPROXY(LockCursor, 67, void, (void));
};
