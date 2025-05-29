#include "render.h"

const Vector& GetMainViewOrigin()
{
	static uintptr_t GetMainViewOriginAddr = Memory::RelativeToReal(Memory::FindSignature("client.dll", Render_GetMainViewOrigin_Call), 1, 5);
	using GetMainViewOriginFn = const Vector& (*)(int _);

	static GetMainViewOriginFn FnGetMainViewOrigin = reinterpret_cast<GetMainViewOriginFn>(GetMainViewOriginAddr);

	return FnGetMainViewOrigin(0);
}
