#include "../GloBals.h"
HANDLE hShare = NULL;
std::string Key = "";
std::string Flag = "";
int Length = 0;

bool OpenShareMemory() {

	hShare = OpenFileMappingA(FILE_MAP_READ, NULL,Flag.c_str());
	if (!hShare) {
		Log(_xor_("[-]Thrad:%x Fail hShare is NULL"), GetCurrentProcessId());
	}
	return hShare != NULL;
}
std::vector<char> GetInfoFromShareMemory() {

	std::vector<char> Buffer;

	if (hShare == NULL) {
		Buffer.clear();
		return Buffer;
	}
	LPVOID lpBase = MapViewOfFile(hShare, FILE_MAP_READ, 0, 0, 0);

	Buffer.resize(Length+1);
	memcpy(Buffer.data(), lpBase, Length);
	Buffer[Length] = '\0';//ÊÖ¶¯½Ø¶Ï
	UnmapViewOfFile(lpBase);

	return Buffer;
}
bool CloseShareMemory() {
	if (hShare == NULL) {
		return false;
	}
	CloseHandle(hShare);
	return true;
}
bool InitCheck() {

	char* TempFlag = new char[100];
	char* TempKey = new char[100];
	char* TempLength = new char[100];
	GetPrivateProfileStringA((const char*)_xor_("Check"), (const char*)_xor_("Flag"), "", TempFlag,100, (const char*)_xor_("C:\\XinYu\\Config.ini"));
	Flag = TempFlag;


	GetPrivateProfileStringA((const char*)_xor_("Check"), (const char*)_xor_("Key"), "", TempKey, 100, (const char*)_xor_("C:\\XinYu\\Config.ini"));
	Key = TempKey;
	

	GetPrivateProfileStringA((const char*)_xor_("Check"), (const char*)_xor_("Length"), "", TempLength,100, (const char*)_xor_("C:\\XinYu\\Config.ini"));
	std::string t_Length = TempLength;
	Length = atoi(t_Length.c_str());

	delete[] TempFlag;
	delete[] TempKey;
	delete[] TempLength;
	if (Flag.length() > 0 && Key.length() > 0 && Length > 0) {
		return true;
	}
	else {
		Log(_xor_("[-]Thrad:%x Fail Check:Something empty"), GetCurrentProcessId());
	}
	return false;
}
bool DoCheckLegal() {
	if (!InitCheck())
		return false;

	if (!OpenShareMemory())
		return false;

	std::vector<char> Data = GetInfoFromShareMemory();
	std::string S_Data = Data.data();
	CloseShareMemory();
	return S_Data == Key;
}