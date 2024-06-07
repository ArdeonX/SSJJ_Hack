#include "AntiCheat.h"
extern Config* g_Config;

typedef enum class D3DX11_IMAGE_FILE_FORMAT {
	D3DX11_IFF_BMP = 0,
	D3DX11_IFF_JPG = 1,
	D3DX11_IFF_PNG = 3,
	D3DX11_IFF_DDS = 4,
	D3DX11_IFF_TIFF = 10,
	D3DX11_IFF_GIF = 11,
	D3DX11_IFF_WMP = 12,
	D3DX11_IFF_FORCE_DWORD = 0x7fffffff
} D3DX11_IMAGE_FILE_FORMAT, * LPD3DX11_IMAGE_FILE_FORMAT;
typedef int (WINAPI* BITBLT)(HDC, int, int, int, int, HDC, int, int, DWORD);
typedef int (WINAPI* SCREENSHOTJPEG)(void*, int);
typedef HRESULT(WINAPI* D3DX11SAVETEXTURETOMEMORY)(ID3D11DeviceContext*, ID3D11Resource*, D3DX11_IMAGE_FILE_FORMAT, LPD3D10BLOB*, UINT);
typedef HRESULT(WINAPI* D3DX11SAVETEXTURETOFILEA)(ID3D11DeviceContext*, ID3D11Resource*, D3DX11_IMAGE_FILE_FORMAT, LPCTSTR);//LPCTSTR
typedef HRESULT(WINAPI* D3DX11SAVETEXTURETOFILEW)(ID3D11DeviceContext*, ID3D11Resource*, D3DX11_IMAGE_FILE_FORMAT, LPCWSTR);//LPCWSTR

BITBLT FnBitBlt = NULL;
SCREENSHOTJPEG FnScreenShotJPEG = NULL;
D3DX11SAVETEXTURETOMEMORY FnD3DX11SaveTextureToMemory = NULL;
D3DX11SAVETEXTURETOFILEA FnD3DX11SaveTextureToFileA = NULL;
D3DX11SAVETEXTURETOFILEW FnD3DX11SaveTextureToFileW = NULL;

std::string GetStringLeft(std::string str, std::string cstr) {
	std::string tmp = str;
	tmp.erase(tmp.find(cstr), tmp.length() - tmp.find(cstr));
	return tmp;
}
//Hooking Dx Capture
HRESULT WINAPI DetourD3DX11SaveTextureToMemory(ID3D11DeviceContext* pContext, ID3D11Resource* pSrcTexture, D3DX11_IMAGE_FILE_FORMAT DestFormat, LPD3D10BLOB* ppDestBuf, UINT Flags) {
	g_Config->CanDraw = false;
	auto Result = FnD3DX11SaveTextureToMemory(pContext, pSrcTexture, DestFormat, ppDestBuf, Flags);//ÈÃËû¼ÌÐø½ØÍ¼
	g_Config->CanDraw = true;
	return Result;

}
HRESULT WINAPI DetourD3DX11SaveTextureToFileA(ID3D11DeviceContext* pContext, ID3D11Resource* pSrcTexture, D3DX11_IMAGE_FILE_FORMAT DestFormat, LPCTSTR pDestFile) {
	g_Config->CanDraw = false;
	auto Result = FnD3DX11SaveTextureToFileA(pContext, pSrcTexture, DestFormat, pDestFile);//ÈÃËû¼ÌÐø½ØÍ¼
	g_Config->CanDraw = true;
	return Result;
}
HRESULT WINAPI DetourD3DX11SaveTextureToFileW(ID3D11DeviceContext* pContext, ID3D11Resource* pSrcTexture, D3DX11_IMAGE_FILE_FORMAT DestFormat, LPCWSTR pDestFile) {
	g_Config->CanDraw = false;
	auto Result = FnD3DX11SaveTextureToFileW(pContext, pSrcTexture, DestFormat, pDestFile);//ÈÃËû¼ÌÐø½ØÍ¼
	g_Config->CanDraw = true;
	return Result;
}
//Hooking Api Capture
BOOL WINAPI  DetourBitBlt(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop) {
	g_Config->CanDraw = false;
	auto Result = FnBitBlt(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);//ÈÃËû¼ÌÐø½ØÍ¼
	g_Config->CanDraw = true;
	return Result;
}
//Hooking static Dll Capture
int WINAPI DetourScreenShotJPEG(void* a, int b) {
	g_Config->CanDraw = false;
	auto Result = FnScreenShotJPEG(a, b);//ÈÃËû¼ÌÐø½ØÍ¼
	g_Config->CanDraw = true;
	return Result;
}
void AntiCheatFn() {
	HMODULE Gdi32Module = 0;
	HMODULE BaseModule = 0;
	HMODULE D3DX11Module = 0;
	HMODULE EyeRecallModule = 0;
	while (Gdi32Module == 0 || BaseModule == 0 || D3DX11Module == 0) {
		Gdi32Module = GetModuleHandleA(_xor_("Gdi32.dll"));
		BaseModule = GetModuleHandleA(_xor_("SSJJ_BattleClient_Unity.exe"));
		D3DX11Module = GetModuleHandleA(_xor_("d3dx11_42.dll"));
		if (!D3DX11Module)
			D3DX11Module = GetModuleHandleA(_xor_("d3dx11_43.dll"));
		if (!D3DX11Module)
			D3DX11Module = LoadLibraryA(_xor_("d3dx11_43.dll"));

	}

	char Temp[MAX_PATH] = { 0 };
	GetModuleFileNameA(BaseModule, Temp, MAX_PATH);
	std::string EyeRecallPath = GetStringLeft(Temp, (const char*)_xor_("SSJJ_BattleClient_Unity.exe")).append((const char*)_xor_("SSJJ_BattleClient_Unity_Data\\Plugins\\EyeRecall.dll"));
	EyeRecallModule = LoadLibraryA(EyeRecallPath.c_str());

	MH_Initialize();
	DWORD Ad_BitBlt = (DWORD)GetProcAddress(Gdi32Module, _xor_("BitBlt"));
	MH_CreateHook((LPVOID)Ad_BitBlt, &DetourBitBlt, reinterpret_cast<LPVOID*>(&FnBitBlt));
	MH_EnableHook((LPVOID)Ad_BitBlt);



	DWORD Ad_ScreenShotJPEG = (DWORD)GetProcAddress(EyeRecallModule, _xor_("ScreenShotJPEG"));
	MH_CreateHook((LPVOID)Ad_ScreenShotJPEG, &DetourScreenShotJPEG, reinterpret_cast<LPVOID*>(&FnScreenShotJPEG));
	MH_EnableHook((LPVOID)Ad_ScreenShotJPEG);

	DWORD Ad_D3DX11SaveTextureToMemory = (DWORD)GetProcAddress(D3DX11Module, _xor_("D3DX11SaveTextureToMemory"));
	MH_CreateHook((LPVOID)Ad_D3DX11SaveTextureToMemory, &DetourD3DX11SaveTextureToMemory, reinterpret_cast<LPVOID*>(&FnD3DX11SaveTextureToMemory));
	MH_EnableHook((LPVOID)Ad_D3DX11SaveTextureToMemory);

	DWORD Ad_D3DX11SaveTextureToFileA = (DWORD)GetProcAddress(D3DX11Module, _xor_("D3DX11SaveTextureToFileA"));
	MH_CreateHook((LPVOID)Ad_D3DX11SaveTextureToFileA, &DetourD3DX11SaveTextureToFileA, reinterpret_cast<LPVOID*>(&FnD3DX11SaveTextureToFileA));
	MH_EnableHook((LPVOID)Ad_D3DX11SaveTextureToFileA);

	DWORD Ad_D3DX11SaveTextureToFileW = (DWORD)GetProcAddress(D3DX11Module, _xor_("D3DX11SaveTextureToFileW"));
	MH_CreateHook((LPVOID)Ad_D3DX11SaveTextureToFileW, &DetourD3DX11SaveTextureToFileW, reinterpret_cast<LPVOID*>(&FnD3DX11SaveTextureToFileW));
	MH_EnableHook((LPVOID)Ad_D3DX11SaveTextureToFileW);

	return;
}