#pragma once

#include "vector.h"
#include "../memory.h"
#include "../signatures.h"
#include <cstdint>

Vector& GetMainViewOrigin()
{
	static uintptr_t GetMainViewOriginAddr = Memory::FindSignature("client.dll", Render_GetMainViewOrigin);
	using GetMainViewOriginFn = const Vector& (*)(int _);

	static GetMainViewOriginFn FnGetMainViewOrigin = reinterpret_cast<GetMainViewOriginFn>(GetMainViewOriginAddr);

	Vector ViewOrigin = FnGetMainViewOrigin(0);
	return ViewOrigin;
}