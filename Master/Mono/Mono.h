#include "../GloBals.h"
#ifndef  _Include_Mono_
#define _Include_Mono_

namespace Mono
{
	std::string GetNameWithClass(void* kclass);
	void* GetClassWithFullName(void* image, const char* full_class_name, const char* class_name);
	/*
		 以下函数接受公开调用

	*/
	void EnumAssembly(PVOID domain, std::vector<PVOID>* Array);
	auto InitMono()->bool;
	auto GetStaticFieldValue(std::string DllStr, std::string ClassStr, std::string FieldStr)->uintptr_t;
	auto GetFieldOffset(std::string DllStr, std::string ClassStr, std::string FieldStr, bool IsStaitc)->uintptr_t;
	auto GetMethod(void* image, const char* spaceName, const char* className, const char* methodName, int parm = -1)->void*;
	auto GetImage(const char* mono_assembly_name) -> void*;
	auto GetClassProperty(std::string DllStr, std::string ClassStr, std::string PropertyStr)->void*;
	//auto GetClassPropertyValue(void* Class, void* Property)->void*;
}


#endif // ! Include_Mono_