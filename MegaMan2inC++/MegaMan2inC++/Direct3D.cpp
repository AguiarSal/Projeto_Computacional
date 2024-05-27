#include "Direct3D.h"

ID3D11Device* Direct3D::Device = nullptr;
ID3D11DeviceContext* Direct3D::Context = nullptr;
D3D11_VIEWPORT Direct3D::Viewport = { 0 };

Direct3D::Direct3D()
{
	SwapChain = nullptr;
	RenderTargetView = nullptr;
	BlendState = nullptr;
	FeatureLevel = D3D_FEATURE_LEVEL_11_0;

	BackgroundColor[0] = 0.0f; // Vermelho
	BackgroundColor[1] = 0.0f; // Verde
	BackgroundColor[2] = 0.0f; // Azul
	BackgroundColor[3] = 0.0f; // Transparência

	vSync = false;
}

Direct3D::~Direct3D()
{
	if (BlendState)
	{
		BlendState->Release();
		BlendState = nullptr;
	}

	if (RenderTargetView)
	{
		RenderTargetView->Release();
		RenderTargetView = nullptr;
	}

	if (Context)
	{
		Context->Release();
		Context = nullptr;
	}

	if (Device)
	{
		Device->Release();
		Device = nullptr;
	}
}

bool Direct3D::Initialize(Window* window)
{
	unsigned int CreateDeviceFlags = 0;

#ifdef _DEBUG
	CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	if FAILED(
		D3D11CreateDevice(
			NULL,                           // adaptador de vídeo (NULL = adaptador padrão)
			D3D_DRIVER_TYPE_HARDWARE,       // tipo de driver D3D (Hardware, Reference ou Software)
			NULL,                           // ponteiro para rasterizador em software
			CreateDeviceFlags,              // modo de depuração ou modo normal
			NULL,                           // featureLevels do Direct3D (NULL = maior suportada)
			0,                              // tamanho do vetor featureLevels
			D3D11_SDK_VERSION,              // versão do SDK do Direct3D
			&Device,                        // guarda o dispositivo D3D criado
			&FeatureLevel,                  // versão do Direct3D utilizada
			&Context))                      // contexto do dispositivo D3D
	{
		if FAILED(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_WARP,
			NULL, CreateDeviceFlags, NULL, 0, D3D11_SDK_VERSION,
			&Device, &FeatureLevel, &Context))
			return false;
		OutputDebugString("---> Usando Adaptador WARP: não há suporte ao D3D11\n");
	}

	COLORREF Color = window->GetWndColor();
	
	BackgroundColor[0] = GetRValue(Color) / 255.0f;
	BackgroundColor[1] = GetGValue(Color) / 255.0f;
	BackgroundColor[2] = GetBValue(Color) / 255.0f;
	BackgroundColor[3] = 1.0f;

	IDXGIDevice* dxgiDevice = nullptr;
	if FAILED(Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice))
		return false;

	IDXGIAdapter* dxgiAdapter = nullptr;
	if FAILED(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter))
		return false;

	IDXGIFactory* dxgiFactory = nullptr;
	if FAILED(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory))
		return false;

	DXGI_SWAP_CHAIN_DESC SwapDesc = { 0 };
	SwapDesc.BufferDesc.Width = unsigned int (window->GetWndWidth());          
	SwapDesc.BufferDesc.Height = unsigned int (window->GetWndHeigth());        
	SwapDesc.BufferDesc.RefreshRate.Numerator = 60;             
	SwapDesc.BufferDesc.RefreshRate.Denominator = 1;            
	SwapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    
	SwapDesc.SampleDesc.Count = 1;                             
	SwapDesc.SampleDesc.Quality = 0;                            
	SwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     
	SwapDesc.BufferCount = 2;                                   
	SwapDesc.OutputWindow = window->GetWndId();                       
	SwapDesc.Windowed = (window->GetWndMode() != FULLSCREEN);         
	SwapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;        
	SwapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	if FAILED(dxgiFactory->CreateSwapChain(Device, &SwapDesc, &SwapChain))
		return false;

	if FAILED(dxgiFactory->MakeWindowAssociation(window->GetWndId(), DXGI_MWA_NO_ALT_ENTER))
		return false;

	ID3D11Texture2D* BackBuffer = nullptr;

	if FAILED(SwapChain->GetBuffer(0, __uuidof(BackBuffer), (void**)(&BackBuffer)))
		return false;

	if FAILED(Device->CreateRenderTargetView(BackBuffer, NULL, &RenderTargetView))
		return false;

	Context->OMSetRenderTargets(1, &RenderTargetView, nullptr);

	Viewport.TopLeftX = 0;
	Viewport.TopLeftY = 0;
	Viewport.Width = float(window->GetWndWidth());
	Viewport.Height = float(window->GetWndHeigth());
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;

	Context->RSSetViewports(1, &Viewport);

	D3D11_BLEND_DESC BlendDesc = { 0 };
	BlendDesc.AlphaToCoverageEnable = false;                                
	BlendDesc.IndependentBlendEnable = false;                               
	BlendDesc.RenderTarget[0].BlendEnable = true;                           
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;             
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;        
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;                 
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;              
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;            
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;            
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;

	if FAILED(Device->CreateBlendState(&BlendDesc, &BlendState))
		return false;

	Context->OMSetBlendState(BlendState, nullptr, 0xffffffff);

	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();
	BackBuffer->Release();

	return true;
}