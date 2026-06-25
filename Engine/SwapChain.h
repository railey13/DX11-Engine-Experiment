#pragma once
#include "d3d11.h"
#include "Prerequisites.h"

class SwapChain {
public:
	SwapChain(HWND hwnd, ui32  width, ui32  height, RenderSystem* system);

	bool present(bool vsync);

	~SwapChain();
private:
	IDXGISwapChain* m_swap_chain;
	ID3D11RenderTargetView* m_rtv;
	RenderSystem* m_system = nullptr;
private:
	friend class DeviceContext;
};

