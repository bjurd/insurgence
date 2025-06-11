#include "vmt.h"

void** VMT::GetVTable(void* Object)
{
	return *reinterpret_cast<void***>(Object);
}

void* VMT::GetMethodPointerAt(void*** Object, uintptr_t Index)
{
	return (*Object)[Index];
}
