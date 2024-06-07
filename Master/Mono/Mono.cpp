#include "Mono.h"
extern Memory* g_Memory;

struct MonoImage
{
	DWORD RawData;
	uint32_t RawDataLen;
	uint32_t UnknownData;
	DWORD RawName;

	auto ToString()->std::string
	{
		char MonoName[256] = { '\0' };
		if (g_Memory->ReadMemory(RawName, &MonoName, 256))
		{
			auto Str = std::string(MonoName);
			auto Position = Str.rfind("\\");
			if (Position != Str.npos)
			{
				Str = Str.substr(Position + 1);
			}
			return Str;
		}
		return (std::string)(const char*)_xor_("NULL");
	}
};
typedef enum {
	MONO_TABLE_MODULE,
	MONO_TABLE_TYPEREF,
	MONO_TABLE_TYPEDEF,
	MONO_TABLE_FIELD_POINTER,
	MONO_TABLE_FIELD,
	MONO_TABLE_METHOD_POINTER,
	MONO_TABLE_METHOD,
	MONO_TABLE_PARAM_POINTER,
	MONO_TABLE_PARAM,
	MONO_TABLE_INTERFACEIMPL,
	MONO_TABLE_MEMBERREF, /* 0xa */
	MONO_TABLE_CONSTANT,
	MONO_TABLE_CUSTOMATTRIBUTE,
	MONO_TABLE_FIELDMARSHAL,
	MONO_TABLE_DECLSECURITY,
	MONO_TABLE_CLASSLAYOUT,
	MONO_TABLE_FIELDLAYOUT, /* 0x10 */
	MONO_TABLE_STANDALONESIG,
	MONO_TABLE_EVENTMAP,
	MONO_TABLE_EVENT_POINTER,
	MONO_TABLE_EVENT,
	MONO_TABLE_PROPERTYMAP,
	MONO_TABLE_PROPERTY_POINTER,
	MONO_TABLE_PROPERTY,
	MONO_TABLE_METHODSEMANTICS,
	MONO_TABLE_METHODIMPL,
	MONO_TABLE_MODULEREF, /* 0x1a */
	MONO_TABLE_TYPESPEC,
	MONO_TABLE_IMPLMAP,
	MONO_TABLE_FIELDRVA,
	MONO_TABLE_UNUSED6,
	MONO_TABLE_UNUSED7,
	MONO_TABLE_ASSEMBLY, /* 0x20 */
	MONO_TABLE_ASSEMBLYPROCESSOR,
	MONO_TABLE_ASSEMBLYOS,
	MONO_TABLE_ASSEMBLYREF,
	MONO_TABLE_ASSEMBLYREFPROCESSOR,
	MONO_TABLE_ASSEMBLYREFOS,
	MONO_TABLE_FILE,
	MONO_TABLE_EXPORTEDTYPE,
	MONO_TABLE_MANIFESTRESOURCE,
	MONO_TABLE_NESTEDCLASS,
	MONO_TABLE_GENERICPARAM, /* 0x2a */
	MONO_TABLE_METHODSPEC,
	MONO_TABLE_GENERICPARAMCONSTRAINT
} MonoMetaTableEnum;
typedef enum {
	MONO_TOKEN_MODULE = 0x00000000,
	MONO_TOKEN_TYPE_REF = 0x01000000,
	MONO_TOKEN_TYPE_DEF = 0x02000000,
	MONO_TOKEN_FIELD_DEF = 0x04000000,
	MONO_TOKEN_METHOD_DEF = 0x06000000,
	MONO_TOKEN_PARAM_DEF = 0x08000000,
	MONO_TOKEN_INTERFACE_IMPL = 0x09000000,
	MONO_TOKEN_MEMBER_REF = 0x0a000000,
	MONO_TOKEN_CUSTOM_ATTRIBUTE = 0x0c000000,
	MONO_TOKEN_PERMISSION = 0x0e000000,
	MONO_TOKEN_SIGNATURE = 0x11000000,
	MONO_TOKEN_EVENT = 0x14000000,
	MONO_TOKEN_PROPERTY = 0x17000000,
	MONO_TOKEN_MODULE_REF = 0x1a000000,
	MONO_TOKEN_TYPE_SPEC = 0x1b000000,
	MONO_TOKEN_ASSEMBLY = 0x20000000,
	MONO_TOKEN_ASSEMBLY_REF = 0x23000000,
	MONO_TOKEN_FILE = 0x26000000,
	MONO_TOKEN_EXPORTED_TYPE = 0x27000000,
	MONO_TOKEN_MANIFEST_RESOURCE = 0x28000000,
	MONO_TOKEN_GENERIC_PARAM = 0x2a000000,
	MONO_TOKEN_METHOD_SPEC = 0x2b000000,
	MONO_TOKEN_STRING = 0x70000000,
	MONO_TOKEN_NAME = 0x71000000,
	MONO_TOKEN_BASE_TYPE = 0x72000000
} MonoTokenType;
typedef void(__cdecl* ThreadAttach)(void*);
typedef PVOID(__cdecl* GetRootDomain)();
typedef PVOID(__cdecl* GetVTableByClass)(PVOID, PVOID);
typedef void* (__cdecl* DomainAssemblyOpen)(void*, const char*);
typedef void(__cdecl* ForeachAssembly)(PVOID, PVOID);
typedef PVOID(__cdecl* GetImageByAssembly)(PVOID);
typedef PVOID(__cdecl* GetTableInfoByImage)(PVOID, int);
typedef int(__cdecl* GetRowsByTableInfo)(PVOID);
typedef PVOID(__cdecl* GetClassByIndex)(PVOID, uint32_t);
typedef char* (__cdecl* GetNameByClass)(PVOID);
typedef PVOID(__cdecl* GetPropertyByName)(void*,const char*);
typedef PVOID(__cdecl* GetFieldByClass)(PVOID, PVOID);
typedef char* (__cdecl* GetNameByField)(PVOID);
typedef PVOID(__cdecl* GetStaticFieldDataByVTable)(PVOID);
typedef int(__cdecl* GetOffsetByField)(PVOID);
typedef int(__cdecl* GetFlagsByField)(PVOID);
typedef PVOID(__cdecl* GetMethodByClass)(PVOID, PVOID);
typedef char* (__cdecl* GetNameByMethod)(PVOID);
typedef void* (__cdecl* GetClassParent)(void*);
typedef void* (__cdecl* GetClassFromName)(void*, const char*, const char*);
typedef void* (__cdecl* GetMethodFromName)(void*, const char*, int);
typedef void* (__cdecl* GetClassMethods)(void*, void*);
typedef void* (__cdecl* GetCompileMethod)(void*);

//mono_class_get_properties
typedef void* (_cdecl* GetClassProperties)(PVOID, PVOID);
typedef void*(__cdecl* GetPropertyFromName)(void*, const char*);
typedef char* (__cdecl* GetNameByProperty)(PVOID);
typedef void* (__cdecl* GetPropertyValue)(void*,void*,void*,void*);
typedef void* (__cdecl* GetPropertyGetMethod)(void*);
typedef void* (__cdecl* PropertyGetObject)(void*, void*, void*);
PVOID RootDomain = nullptr;
GetRootDomain Mono_GetRootDomain = nullptr;
GetVTableByClass Mono_GetVTableByClass = nullptr;
ForeachAssembly Mono_ForeachAssembly = nullptr;
GetImageByAssembly Mono_GetImageByAssembly = nullptr;
GetTableInfoByImage Mono_GetTableInfoByImage = nullptr;
GetRowsByTableInfo Mono_GetRowsByTableInfo = nullptr;
GetClassByIndex Mono_GetClassByIndex = nullptr;
GetNameByClass Mono_GetNameByClass = nullptr;
GetFieldByClass Mono_GetFieldByClass = nullptr;
GetNameByField Mono_GetNameByField = nullptr;
GetStaticFieldDataByVTable Mono_GetStaticFieldDataByVTable = nullptr;
GetOffsetByField Mono_GetOffsetByField = nullptr;
GetFlagsByField Mono_GetFlagsByField = nullptr;
GetMethodByClass Mono_GetMethodByClass = nullptr;
GetNameByMethod Mono_GetNameByMethod = nullptr;
ThreadAttach Mono_ThreadAttach = nullptr;
DomainAssemblyOpen Mono_DomainAssemblyOpen = nullptr;
GetClassParent Mono_GetClassParent = nullptr;
GetClassFromName Mono_GetClassFromName = nullptr;
GetMethodFromName Mono_GetMethodFromName = nullptr;
GetClassMethods Mono_GetClassMethods = nullptr;
GetCompileMethod Mono_GetCompileMethod = nullptr;
GetPropertyFromName Mono_GetPropertyFromName = nullptr;
GetClassProperties Mono_GetClassProperties = nullptr;
GetNameByProperty Mono_GetNameByProperty = nullptr;
GetPropertyValue Mono_GetPropertyValue = nullptr;
GetPropertyGetMethod 	Mono_GetPropertyGetMethod = nullptr;
PropertyGetObject Mono_PropertyGetObject = nullptr;
void Mono::EnumAssembly(PVOID domain, std::vector<PVOID>* Array)
{
	Array->push_back(domain);
}

auto Mono::InitMono()->bool
{
	auto MonoHandle = GetModuleHandleA(_xor_("mono.dll"));
	if (!MonoHandle) {
		return false;
	}
	Mono_GetRootDomain = (GetRootDomain)GetProcAddress(MonoHandle, _xor_("mono_get_root_domain"));
	Mono_ThreadAttach = (ThreadAttach)GetProcAddress(MonoHandle, _xor_("mono_thread_attach"));
	Mono_DomainAssemblyOpen = (DomainAssemblyOpen)GetProcAddress(MonoHandle, _xor_("mono_domain_assembly_open"));
	Mono_GetVTableByClass = (GetVTableByClass)GetProcAddress(MonoHandle, _xor_("mono_class_vtable"));
	Mono_ForeachAssembly = (ForeachAssembly)GetProcAddress(MonoHandle, _xor_("mono_assembly_foreach"));
	Mono_GetImageByAssembly = (GetImageByAssembly)GetProcAddress(MonoHandle, _xor_("mono_assembly_get_image"));
	Mono_GetTableInfoByImage = (GetTableInfoByImage)GetProcAddress(MonoHandle, _xor_("mono_image_get_table_info"));
	Mono_GetRowsByTableInfo = (GetRowsByTableInfo)GetProcAddress(MonoHandle, _xor_("mono_table_info_get_rows"));
	Mono_GetClassByIndex = (GetClassByIndex)GetProcAddress(MonoHandle, _xor_("mono_class_get"));
	Mono_GetNameByClass = (GetNameByClass)GetProcAddress(MonoHandle, _xor_("mono_class_get_name"));
	Mono_GetClassParent = (GetClassParent)GetProcAddress(MonoHandle, _xor_("mono_class_get_parent"));
	Mono_GetFieldByClass = (GetFieldByClass)GetProcAddress(MonoHandle, _xor_("mono_class_get_fields"));
	Mono_GetNameByField = (GetNameByField)GetProcAddress(MonoHandle, _xor_("mono_field_get_name"));
	Mono_GetStaticFieldDataByVTable = (GetStaticFieldDataByVTable)GetProcAddress(MonoHandle, _xor_("mono_vtable_get_static_field_data"));
	Mono_GetClassFromName = (GetClassFromName)GetProcAddress(MonoHandle, _xor_("mono_class_from_name"));
	Mono_GetMethodFromName = (GetMethodFromName)GetProcAddress(MonoHandle, _xor_("mono_class_get_method_from_name"));
	Mono_GetClassMethods = (GetClassMethods)GetProcAddress(MonoHandle, _xor_("mono_class_get_methods"));
	Mono_GetCompileMethod = (GetCompileMethod)GetProcAddress(MonoHandle, _xor_("mono_compile_method"));
	Mono_GetOffsetByField = (GetOffsetByField)GetProcAddress(MonoHandle, _xor_("mono_field_get_offset"));
	Mono_GetFlagsByField = (GetFlagsByField)GetProcAddress(MonoHandle, _xor_("mono_field_get_flags"));
	Mono_GetPropertyFromName = (GetPropertyFromName)GetProcAddress(MonoHandle, _xor_("mono_class_get_property_from_name"));
	Mono_GetClassProperties= (GetClassProperties)GetProcAddress(MonoHandle, _xor_("mono_class_get_properties"));
	Mono_GetNameByProperty = (GetNameByProperty)GetProcAddress(MonoHandle, _xor_("mono_property_get_name"));
	Mono_GetPropertyValue=(GetPropertyValue)GetProcAddress(MonoHandle, _xor_("mono_property_get_value"));
	Mono_GetPropertyGetMethod=(GetPropertyGetMethod)GetProcAddress(MonoHandle, _xor_("mono_property_get_get_method"));
	Mono_PropertyGetObject=(PropertyGetObject)GetProcAddress(MonoHandle, _xor_("mono_property_get_object"));
	RootDomain = Mono_GetRootDomain();
	if (!RootDomain) {
		return false;
	}
	return true;
}

auto Mono::GetStaticFieldValue(std::string DllStr, std::string ClassStr, std::string FieldStr)->uintptr_t
{
	std::vector<PVOID> AssemblyArray;
	Mono_ForeachAssembly(EnumAssembly, &AssemblyArray);
	for (auto i = 0; i < AssemblyArray.size(); i++)
	{
		auto Assembly = AssemblyArray[i];
		if (!Assembly) {
			continue;
		}
		auto Image = Mono_GetImageByAssembly(Assembly);
		if (!Image) {
			continue;
		}
		auto MonoStruct = g_Memory->Read<MonoImage>((uintptr_t)Image + 0x10);
		auto MonoRawName = MonoStruct.ToString();
		if (MonoRawName == DllStr)
		{
			auto TableInfo = Mono_GetTableInfoByImage(Image, MONO_TABLE_TYPEDEF);
			if (!TableInfo) {
				continue;
			}
			auto TableNum = Mono_GetRowsByTableInfo(TableInfo);
			for (auto i = 0; i < TableNum; i++)
			{
				auto Class = Mono_GetClassByIndex(Image, MONO_TOKEN_TYPE_DEF | i + 1);
				if (!Class) {
					continue;
				}
				auto ClassName = Mono_GetNameByClass(Class);
				if (ClassStr == ClassName)
				{
					PVOID Iter = NULL, Field;
					do
					{
						Field = Mono_GetFieldByClass(Class, &Iter);
						if (Field)
						{
							auto FieldName = Mono_GetNameByField(Field);
							if (FieldStr == FieldName)
							{
								auto VTable = (RootDomain && Class) ? Mono_GetVTableByClass(RootDomain, Class) : nullptr;
								if (VTable)
								{
									return (uintptr_t)Mono_GetStaticFieldDataByVTable(VTable);
								}
							}
						}
					} while (Field);
				}
			}
		}
	}
	return NULL;
}

auto Mono::GetFieldOffset(std::string DllStr, std::string ClassStr, std::string FieldStr, bool IsStaitc)->uintptr_t
{
	std::vector<PVOID> AssemblyArray;
	Mono_ForeachAssembly(EnumAssembly, &AssemblyArray);
	for (auto i = 0; i < AssemblyArray.size(); i++)
	{
		auto Assembly = AssemblyArray[i];
		if (!Assembly) { continue; }
		auto Image = Mono_GetImageByAssembly(Assembly);
		if (!Image) { continue; }
		auto MonoStruct = g_Memory->Read<MonoImage>((uintptr_t)Image + 0x10);
		auto MonoRawName = MonoStruct.ToString();
		if (MonoRawName == DllStr)
		{
			auto TableInfo = Mono_GetTableInfoByImage(Image, MONO_TABLE_TYPEDEF);
			if (!TableInfo) { continue; }
			auto TableNum = Mono_GetRowsByTableInfo(TableInfo);
			for (auto i = 0; i < TableNum; i++)
			{
				auto Class = Mono_GetClassByIndex(Image, MONO_TOKEN_TYPE_DEF | i + 1);
				if (!Class) { continue; }
				auto ClassName = Mono_GetNameByClass(Class);
				//std::cout << "ClassName:" << ClassName << std::endl;
				if (ClassStr == ClassName)
				{
					PVOID Iter = NULL, Field;
					do
					{
						Field = Mono_GetFieldByClass(Class, &Iter);
						if (Field)
						{
							if (IsStaitc && (Mono_GetFlagsByField(Field) & 0x10))
							{

								auto FieldName = Mono_GetNameByField(Field);
								//printf("FieldName:%s Offsets:%x\n", FieldName, Mono_GetOffsetByField(Field));
								if (FieldStr == FieldName)
								{
									return Mono_GetOffsetByField(Field);
								}
							}
							else
							{
								auto FieldName = Mono_GetNameByField(Field);
							 // printf("FieldName:%s Offsets:%x\n", FieldName, Mono_GetOffsetByField(Field));
								
								if (FieldStr == FieldName)
								{
									return Mono_GetOffsetByField(Field);
								}
							}
						}
					} while (Field);
				}
			}
		}
	}
	return NULL;
}

std::string Mono::GetNameWithClass(void* kclass)
{
	return (std::string)Mono_GetNameByClass(kclass);
}

void* Mono::GetClassWithFullName(void* image, const char* full_class_name, const char* class_name)
{
	void* table_info = Mono_GetTableInfoByImage(image, 2);
	if (table_info) {
		int table_rows = Mono_GetRowsByTableInfo(table_info);
		for (int i = 0; i < table_rows; i++) {
			void* kclass = Mono_GetClassByIndex(image, 0x02000000 | i + 1);
			if (kclass != NULL) {
				char* kclass_name = Mono_GetNameByClass(kclass);
				if (strcmp(kclass_name, class_name) == 0) {
					std::string full_name = kclass_name;
					void* pclass = kclass;
					do {
						pclass = Mono_GetClassParent(pclass);
						if (pclass != NULL) {
							std::string parent_name =GetNameWithClass(pclass);
							if (parent_name == (std::string)(const char*)_xor_("MonoBehaviour") 
								|| parent_name == (std::string)(const char*)_xor_("Behaviour")
								|| parent_name == (std::string)(const char*)_xor_("Object") 
								|| parent_name == (std::string)(const char*)_xor_("Component"))
								break;
							full_name = "." + full_name;
							full_name = parent_name + full_name;
						}
					} while (pclass != NULL);
					if (full_name.find(full_class_name) != std::string::npos)
						return kclass;
				}
			}
		}
	}
	return 0;
}

auto Mono::GetMethod(void* image, const char* spaceName, const char* className, const char* methodName, int parm)->void*
{

	void* _class = Mono_GetClassFromName(image, spaceName, className);

	if (!_class) {
		_class = GetClassWithFullName(image, className, className);
		if (!_class)
			return NULL;
	}

	auto method = Mono_GetMethodFromName(_class, methodName, parm);
	if (!method) {
		void* iter = NULL;
		while (TRUE) {
			void* mth = Mono_GetClassMethods(_class, &iter);
			if (!mth)	
				break;
			const char* mth_name = ((const char* (__cdecl*)(void*, bool))(Mono_GetClassMethods))(mth, true);
			if (!strcmp(methodName, mth_name)) {
				method = mth;
			}
			if (method)
				break;
		}
	}
	return Mono_GetCompileMethod(method);
}
auto Mono::GetImage(const char* mono_assembly_name) -> void*
{
	Mono_ThreadAttach(RootDomain);
	auto ase_assembly = Mono_DomainAssemblyOpen(RootDomain, mono_assembly_name);
	return Mono_GetImageByAssembly(ase_assembly);
}
auto Mono::GetClassProperty(std::string DllStr, std::string ClassStr, std::string PropertyStr)->void* {

	std::vector<PVOID> AssemblyArray;
	Mono_ForeachAssembly(EnumAssembly, &AssemblyArray);
	for (auto i = 0; i < AssemblyArray.size(); i++)
	{
		auto Assembly = AssemblyArray[i];
		if (!Assembly) { continue; }
		auto Image = Mono_GetImageByAssembly(Assembly);
		if (!Image) { continue; }
		auto MonoStruct = g_Memory->Read<MonoImage>((uintptr_t)Image + 0x10);
		auto MonoRawName = MonoStruct.ToString();
		if (MonoRawName == DllStr)
		{
			auto TableInfo = Mono_GetTableInfoByImage(Image, MONO_TABLE_TYPEDEF);
			if (!TableInfo) { continue; }
			auto TableNum = Mono_GetRowsByTableInfo(TableInfo);
			for (auto i = 0; i < TableNum; i++)
			{
				auto Class = Mono_GetClassByIndex(Image, MONO_TOKEN_TYPE_DEF | i + 1);
				if (!Class) { continue; }
				auto ClassName = Mono_GetNameByClass(Class);
				if (ClassStr == ClassName)
				{
					PVOID Iter = NULL, Property;
					do
					{
						Property = Mono_GetClassProperties(Class, &Iter);
						if (Property)
						{
						
				         	auto PropertyName =Mono_GetNameByProperty(Property);
							if (PropertyStr == PropertyName)
							{
								//printf("PropertyName: % s Offsets : % x\n", PropertyName, Property);
								//auto get_Method=Mono_GetPropertyGetMethod(Property);
								//printf("Property:%p\n", Property);
								return Property;
							}

						}
					} while (Property);
				}
			}
		}
	}
	return NULL;
}
//auto Mono::GetClassPropertyValue(void* Class, void* Property)->void* {
//   // void* exc = NULL;
//	//mono_property_get_object(MonoDomain * domain, MonoClass * klass, MonoProperty * property);
//	//mono_property_get_value (MonoProperty *prop, void *obj, void **params, MonoObject **exc); 
//	//void* kClass = Mono_GetClassParent(Class);
//	
//	//printf("Object:%p", Object);
//	auto  Value = Mono_GetPropertyValue(Property, Class,NULL, NULL);
////	printf("Value:%p", Value);
//	return Value;
//}