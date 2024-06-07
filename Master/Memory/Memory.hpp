#pragma once
#ifndef _Include_Memory_
#define _Include_Memory_
#include "MinHook/MinHook.h"
void RemovePeHeader(HMODULE hModule);
BOOL CheckLoadFile();
class Memory
{
public:
	auto ReadMemory(DWORD ptr, void* buff, size_t size)->bool
	{
		if ((ptr > 0x1000 - 0x8) || (ptr < 0x7FFFFFFFFFFF + 0x8))
		{
			if (IsBadReadPtr((void*)ptr, size) == 0)
			{
				CopyMemory(buff, (void*)ptr, size);
				return true;
			}
			else
			{
				ZeroMemory(buff, size);
			}
		}
		return false;
	}

	template<class T>
	T Read(DWORD ptr)
	{
		T buff;
		ReadMemory(ptr, &buff, sizeof(T));
		return buff;
	}

	auto WriteMemory(DWORD ptr, void* buff, size_t size)->bool
	{
		if ((ptr > 0x1000 - 0x8) || (ptr < 0x7FFFFFFFFFFF + 0x8))
		{
			if (IsBadReadPtr((void*)ptr, sizeof(size)) == 0)
			{
				CopyMemory((void*)ptr, buff, size);
				return true;
			}
		}
		return false;
	}

	template<class T>
	void Write(DWORD ptr, T Val)
	{
		WriteMemory(ptr, &Val, sizeof(T));
	}


	template< typename ReturnType = void, typename... Args >
	ReturnType CALL(uintptr_t ufn, Args... Arguments)
	{
		return reinterpret_cast<ReturnType(*)(Args...)>(ufn)(Arguments...);
	}

};
#endif