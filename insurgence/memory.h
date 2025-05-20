#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <cstdint>

class Memory
{
public:
	void ParseSignature(const std::string& Signature, std::vector<uint8_t>& Pattern, std::vector<bool>& Mask);
	uintptr_t ScanMemory(uintptr_t Base, size_t Size, std::vector<uint8_t>& Pattern, std::vector<bool>& Mask);
	bool GetModuleInfo(const wchar_t* Name, uintptr_t& Base, size_t& Size);
	uintptr_t FindSignature(const std::string Module, const std::string Signature);
};