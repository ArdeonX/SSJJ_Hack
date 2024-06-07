#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <urlmon.h>
#include <objidl.h>
#include <shlwapi.h>
#include <WinInet.h>
#include <istream>

#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "wininet.lib")


using namespace std;
bool URLDownLoadFileToMemory(std::string Url, std::vector<char>* FileData, IBindStatusCallback* CallBack)//把文件流发回去 需要手动释放
{
	DeleteUrlCacheEntryA(Url.c_str());//否则会从ie拷贝
	IStream* pStream; // 存后面的流
	HRESULT bRet = URLOpenBlockingStreamA(NULL, Url.c_str(), &pStream, NULL, CallBack);
	if (bRet != S_OK) {
		return false;
	}

	ULARGE_INTEGER liSize;
	IStream_Size(pStream, &liSize);
	FileData->resize(liSize.QuadPart);
	DWORD readBytes = 0;  // 读取的字节数

	do
	{
		pStream->Read( // 读取流中的数据
			FileData->data(),     // 缓冲区
			FileData->size(),       // 缓冲区大小
			&readBytes  // 用来存储可读取到的字节数。
		);
	} while (readBytes > 0); // 直到读取完毕（读取字节数为 0）
	pStream->Release();
	return true;
}

BOOL CheckLoadFile()
{


	std::vector<char> File;
	URLDownLoadFileToMemory("http://47.94.159.94:10240/Safe.txt", &File, nullptr);//从服务器给下发一文件
	auto st = new char[File.size() + 1];
	memcpy(st, File.data(), File.size());
	st[File.size()] = '\0';//手动截断

	std::string Te = st;

	return Te == "TRUE";
}