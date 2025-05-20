#include "memory.h"

#include <Windows.h>
#include <Psapi.h>

void Memory::ParseSignature(const std::string& Signature, std::vector<uint8_t>& Pattern, std::vector<bool>& Mask)
{
	std::istringstream Stream(Signature);
	std::string Token;

	while (Stream >> Token)
	{
		if (Token == "?" || Token == "??")
		{
			Pattern.push_back(0x00);
			Mask.push_back(true);
		}
		else
		{
			uint8_t Byte = static_cast<uint8_t>(std::stoi(Token, nullptr, 16));

			Pattern.push_back(Byte);
			Mask.push_back(false);
		}
	}
}

uintptr_t Memory::ScanMemory(uintptr_t Base, size_t Size, std::vector<uint8_t>& Pattern, std::vector<bool>& Mask)
{
	size_t Length = Pattern.size();

	if (Length == 0 || Size < Length)
		return 0;

	for (size_t i = 0; i <= Size - Length; ++i)
	{
		bool Match = true;

		for (size_t j = 0; j < Length; ++j)
		{
			if (!Mask[j] && Pattern[j] != *reinterpret_cast<uint8_t*>(Base + i + j))
			{
				Match = false;
				break;
			}
		}

		if (Match)
			return Base + i;
	}

	return 0;
}

bool Memory::GetModuleInfo(const wchar_t* Name, uintptr_t& Base, size_t& Size)
{
	HMODULE Module = GetModuleHandleW(Name);

	if (!Module)
		return false;

	MODULEINFO Info = { 0 };

	if (!GetModuleInformation(GetCurrentProcess(), Module, &Info, sizeof(Info)))
		return false;

	Base = reinterpret_cast<uintptr_t>(Info.lpBaseOfDll);
	Size = static_cast<size_t>(Info.SizeOfImage);

	return true;
}

uintptr_t Memory::FindSignature(const std::string Module, const std::string Signature)
{
	std::vector<uint8_t> Pattern;
	std::vector<bool> Mask;
	this->ParseSignature(Signature, Pattern, Mask);

	uintptr_t Base = 0;
	size_t Size = 0;

	std::wstring ModuleW; // Dumb
	ModuleW.assign(Module.begin(), Module.end());

	if (!this->GetModuleInfo(ModuleW.c_str(), Base, Size))
		return 0;

	return this->ScanMemory(Base, Size, Pattern, Mask);
}
