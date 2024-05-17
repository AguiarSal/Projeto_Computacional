#ifndef _DIRECT3D_
#define _DIRECT3D_

#include <dxgi.h>
#include <d3d11.h>
#include "Window.h"

class Direct3D
{
private:
	IDXGISwapChain* SwapChain;
	ID3D11RenderTargetView* RenderTargetView;
	ID3D11BlendState* BlendState;
	D3D_FEATURE_LEVEL FeatureLevel;
	float BackgroundColor[4];
	bool vSync;

public:
	Direct3D();
	~Direct3D();

	static ID3D11Device* Device;
	static ID3D11DeviceContext* Context;
	static D3D11_VIEWPORT Viewport;

	void ActiveVSync(bool State);
	void Clear();
	void Present();

	bool Initialize(Window* window);
};

inline void Direct3D::ActiveVSync(bool State) {
	vSync = State;
}

inline void Direct3D::Clear() {
	Context->ClearRenderTargetView(RenderTargetView, BackgroundColor);
}

inline void Direct3D::Present() {
	SwapChain->Present(vSync, NULL);
	Context->OMSetRenderTargets(1, &RenderTargetView, nullptr);
}

#endif
