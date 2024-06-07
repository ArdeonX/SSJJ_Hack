#include "../GloBals.h"
void RemovePeHeader(HMODULE hModule)
{
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((PBYTE)pDosHeader + (DWORD)pDosHeader->e_lfanew);

	if (pNTHeader->Signature != IMAGE_NT_SIGNATURE)
	{
		return;
	}

	if (pNTHeader->FileHeader.SizeOfOptionalHeader)
	{
		DWORD Protect;
		WORD Size = pNTHeader->FileHeader.SizeOfOptionalHeader;
		VirtualProtect((void*)hModule, Size, PAGE_EXECUTE_READWRITE, &Protect);
		SecureZeroMemory((void*)hModule, Size);
		VirtualProtect((void*)hModule, Size, Protect, &Protect);
	}
}