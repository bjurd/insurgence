#include "vmt.h"

void** VMT::GetVTable(void* Object)
{
	return *reinterpret_cast<void***>(Object);
}

char* VMT::GetMethodPointerAt(char*** Object, uintptr_t Index)
{
	return (*Object)[Index];
}
