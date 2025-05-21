#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <cstdint>
#include <Windows.h>

typedef void* (*CreateInterfaceFn)(const char*, int*);

class Memory
{
public:
	static void ParseSignature(const std::string& Signature, std::vector<uint8_t>& Pattern, std::vector<bool>& Mask);
	static uintptr_t ScanMemory(uintptr_t Base, size_t Size, std::vector<uint8_t>& Pattern, std::vector<bool>& Mask);
	static bool GetModuleInfo(const wchar_t* Name, uintptr_t& Base, size_t& Size);
	static uintptr_t FindSignature(const std::string Module, const std::string Signature);

	template <typename T>
	static T RelativeToReal(char* Address, int Offset, int InstructionSize)
	{
		if (!Address) return nullptr;

		char* Instruction = Address + Offset;
		int RelativeAddress = *(int*)(Instruction);

		char* RealAddress = Address + InstructionSize + RelativeAddress;

		return (T)RealAddress;
	}

	template <typename T>
	static T GetInterface(const std::string ModuleName, const std::string InterfaceName)
	{
		HMODULE Module = GetModuleHandleA(ModuleName.c_str());

		if (!Module)
			return nullptr;

		FARPROC Address = GetProcAddress(Module, "CreateInterface");

		if (!Address)
			return nullptr;

		CreateInterfaceFn fnCreateInterface = reinterpret_cast<CreateInterfaceFn>(Address);

		return (T)fnCreateInterface(InterfaceName.c_str(), nullptr);
	}
};
