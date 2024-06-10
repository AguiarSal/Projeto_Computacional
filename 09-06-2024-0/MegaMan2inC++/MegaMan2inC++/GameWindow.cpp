#include <windows.h>
#include <windowsx.h>

#include "Graphics.h"

// variáveis globais e outras coisas relacionadas a criação de janela
int WindowSizeX = 1080;			//tamanho X da janela
int WindowSizeY = 720;			//tamanho Y da janela
bool fullscreen = false;		//determina se o jogo estará em tela cheia
HWND hwnd;						//identificador de janela

HDC hdc;						//contexto do dispositivo gráfico

// variáveis globais relacionadas a detecção de mouse e teclado 
bool vkKeys[256] = { 0 };		//vetor que guarda o estado das teclas do teclado
int MouseX = 0, MouseY = 0;		//posição do mouse
int MouseWheel = 0;				//roda do mouses
bool MouseMiddle = 0;			//botão da rodinha do mouse
bool MouseLeft = 0;				//botão esquerdo do mouse
bool MouseRight = 0;			//botão direito do mouse

void GameInitialization()
{

}

void GameUpdate()
{
	if (vkKeys[VK_ESCAPE])
	{
		PostMessage(hwnd, WM_DESTROY, 0, 0);
	}
}

void GameDraw()
{

}

void GameFinalize()
{

}

LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	MSG				msg;
	WNDCLASS		wndclass;

	wndclass.style			= CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc	= WinProc;
	wndclass.cbClsExtra		= 0;
	wndclass.cbWndExtra		= 0;
	wndclass.hInstance		= hInstance;
	wndclass.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
	wndclass.hCursor		= LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR));	
	wndclass.hbrBackground	= (HBRUSH) GetStockObject (BLACK_BRUSH);
	wndclass.lpszMenuName	= NULL;
	wndclass.lpszClassName	= "MegaWindow";

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, "Error at the Window creation!", "MegaMan2C++", MB_ICONERROR);
		return 0;
	}

	int WindowStyle = WS_OVERLAPPED | WS_SYSMENU;

	if (fullscreen)
	{
		WindowSizeX = GetSystemMetrics(SM_CXSCREEN);
		WindowSizeY = GetSystemMetrics(SM_CYSCREEN);
		WindowStyle = WS_POPUP;
	}

	hwnd = CreateWindowEx(
			NULL,
			"MegaWindow",
			"MegaMan2inC++",
			WindowStyle,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			WindowSizeX,
			WindowSizeY,
			NULL,
			NULL,
			hInstance,
			NULL);
	if (!fullscreen)
	{
		//Criação do retângulo da janela
		RECT WindowRect = { 0, 0, WindowSizeX, WindowSizeY};

		AdjustWindowRectEx(&WindowRect,
						   GetWindowStyle(hwnd),
						   GetMenu(hwnd)!=NULL,
						   GetWindowExStyle(hwnd));

		//posição inicial X da janela
		int WindowPosX = (GetSystemMetrics(SM_CXSCREEN)/2) - ((WindowRect.right-WindowRect.left)/2);
		//posição inicial Y da janela
		int WindowPosY = (GetSystemMetrics(SM_CYSCREEN)/2) - ((WindowRect.bottom-WindowRect.top)/2);

		MoveWindow(
			hwnd,
			WindowPosX,
			WindowPosY,
			WindowRect.right-WindowRect.left,
			WindowRect.bottom-WindowRect.top,
			TRUE);
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	GameInitialization();

	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			GameUpdate();
			GameDraw();
			Sleep(0);
		}
	}
	while (msg.message != WM_QUIT);

	GameFinalize();

	return int(msg.wParam);
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		vkKeys[wParam] = true;
		return 0;

	case WM_KEYUP:
		vkKeys[wParam] = false;
		return 0;

	case WM_MOUSEMOVE:
		MouseX = LOWORD(lParam);
		MouseY = HIWORD(lParam);
		return 0;

	case WM_MOUSEWHEEL:
		MouseWheel = GET_WHEEL_DELTA_WPARAM(wParam);
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_MBUTTONDOWN:
		MouseMiddle = true;
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
	case WM_MBUTTONUP:
		MouseMiddle = false;
		return 0;

	case WM_LBUTTONDOWN:
		MouseLeft = true;
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
	case WM_LBUTTONUP:
		MouseLeft = false;
		return 0;

	case WM_RBUTTONDOWN:
		MouseRight = true;
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
	case WM_RBUTTONUP:
		MouseRight = false;
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}