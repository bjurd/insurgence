#include "render.h"

const Vector& GetMainViewOrigin()
{
	static uintptr_t GetMainViewOriginAddr = Memory::RelativeToReal(Memory::FindSignature("client.dll", Render_GetMainViewOrigin_Call), 1, 5);
	using GetMainViewOriginFn = const Vector& (*)(int _);

	static GetMainViewOriginFn FnGetMainViewOrigin = reinterpret_cast<GetMainViewOriginFn>(GetMainViewOriginAddr);

	return FnGetMainViewOrigin(0);
}

const Angle& GetMainViewAngles()
{
	static uintptr_t GetMainViewAnglesAddr = Memory::RelativeToReal(Memory::FindSignature("client.dll", Render_GetMainViewAngles_Call), 1, 5);
	using GetMainViewAnglesFn = const Angle& (*)(int _);

	static GetMainViewAnglesFn FnGetMainViewAngles = reinterpret_cast<GetMainViewAnglesFn>(GetMainViewAnglesAddr);

	return FnGetMainViewAngles(0);
}
