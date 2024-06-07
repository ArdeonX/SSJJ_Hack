#include "kiero/kiero.h"
#include <Windows.h>
#include <algorithm>
#include <d3d11.h>
#include <functional>
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "../Crypter/xorstr.hpp"
#include <string>
#include <thread>
#include <map>


typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef HRESULT(__stdcall* ResizeBuffers)(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
typedef ULONG(__stdcall* Release)(IDXGISwapChain* pSwapChain);

LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool InitDirectxFn = false;

HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView=NULL;
std::function<void(void)> ThisFn = nullptr;


Present original_Present = nullptr;
ResizeBuffers  original_ResizeBuffers = nullptr;
Release original_Release = nullptr;

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}
HRESULT __stdcall HookResizeBuffersFn(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{

	if (mainRenderTargetView) {
		pContext->OMSetRenderTargets(0, 0, 0);
		mainRenderTargetView->Release();
	}

	HRESULT hr = original_ResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);

	ID3D11Texture2D* pBuffer;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);
	// Perform error handling here!

	pDevice->CreateRenderTargetView(pBuffer, NULL, &mainRenderTargetView);
	// Perform error handling here!
	pBuffer->Release();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);

	// Set up the viewport.
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)Width;
	vp.Height = (FLOAT)Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1, &vp);
	return hr;
}
HRESULT __stdcall HookPresentFn(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{

	if (!InitDirectxFn)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{

			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);

			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
			//io.MouseDrawCursor = true;
			io.Fonts->AddFontFromFileTTF(_xor_("C:\\Windows\\Fonts\\simkai.ttf"), 15.0f, 0, io.Fonts->GetGlyphRangesChineseFull());

			ImGui_ImplWin32_Init(window);
			ImGui_ImplDX11_Init(pDevice, pContext);


			InitDirectxFn = true;
		}

		else
			return original_Present(pSwapChain, SyncInterval, Flags);
	}


	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ThisFn();

	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return  original_Present(pSwapChain, SyncInterval, Flags);

}

bool CreatDirectxHook(std::function<void(void)> CallBackFn) {

	if (!CallBackFn)
		return false;
	ThisFn = CallBackFn;

	if (kiero::init(kiero::RenderType::D3D11) != kiero::Status::Success)
		return false;

	
	if (kiero::bind(8, (void**)&original_Present, HookPresentFn) != kiero::Status::Success)
		return false;

	if (kiero::bind(13, (void**)&original_ResizeBuffers, HookResizeBuffersFn) != kiero::Status::Success)
		return false;


	return true;
}


