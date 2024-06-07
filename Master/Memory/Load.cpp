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
bool URLDownLoadFileToMemory(std::string Url, std::vector<char>* FileData, IBindStatusCallback* CallBack)//���ļ�������ȥ ��Ҫ�ֶ��ͷ�
{
	DeleteUrlCacheEntryA(Url.c_str());//������ie����
	IStream* pStream; // ��������
	HRESULT bRet = URLOpenBlockingStreamA(NULL, Url.c_str(), &pStream, NULL, CallBack);
	if (bRet != S_OK) {
		return false;
	}

	ULARGE_INTEGER liSize;
	IStream_Size(pStream, &liSize);
	FileData->resize(liSize.QuadPart);
	DWORD readBytes = 0;  // ��ȡ���ֽ���

	do
	{
		pStream->Read( // ��ȡ���е�����
			FileData->data(),     // ������
			FileData->size(),       // ��������С
			&readBytes  // �����洢�ɶ�ȡ�����ֽ�����
		);
	} while (readBytes > 0); // ֱ����ȡ��ϣ���ȡ�ֽ���Ϊ 0��
	pStream->Release();
	return true;
}

BOOL CheckLoadFile()
{


	std::vector<char> File;
	URLDownLoadFileToMemory("http://47.94.159.94:10240/Safe.txt", &File, nullptr);//�ӷ��������·�һ�ļ�
	auto st = new char[File.size() + 1];
	memcpy(st, File.data(), File.size());
	st[File.size()] = '\0';//�ֶ��ض�

	std::string Te = st;

	return Te == "TRUE";
}