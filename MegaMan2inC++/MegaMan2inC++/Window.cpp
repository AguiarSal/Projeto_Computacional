#include "Window.h"

bool  Window::WndKeys[256] = { 0 };
int Window::WndMouseXCoord = 0;
int Window::WndMouseYCoord = 0;

Window::Window()
{
	hInstance = GetModuleHandle(NULL);
	hWND = 0;
	WndWidth = GetSystemMetrics(SM_CXSCREEN);
	WndHeight = GetSystemMetrics(SM_CYSCREEN);
	WndIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndCursor = LoadCursor(NULL, IDC_ARROW);
	WndColor = RGB(0, 0, 0);
	WndTitle = string("Game");
	WndStyle = WS_POPUP | WS_VISIBLE;
	WndMode = FULLSCREEN;
	WndInitialPosX = 0;
	WndInitialPosY = 0;
	WndCenterX = WndWidth/2.0f;
	WndCenterY = WndHeight/2.0f;
}

void Window::SetWndMode(int Mode)
{
	WndMode = Mode;

	if (Mode == WINDOWED)
	{
		WndStyle = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE;
	}
	else
	{
		WndStyle = WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE;
	}
}

void Window::SetWndSize(int Width, int Height)
{
	WndWidth = Width;
	WndHeight = Height;

	WndCenterX = WndWidth / 2.0f;
	WndCenterY = WndHeight / 2.0f;

	WndInitialPosX = GetSystemMetrics(SM_CXSCREEN) / 2 - WndWidth / 2;
	WndInitialPosY = GetSystemMetrics(SM_CYSCREEN) / 2 - WndHeight / 2;
}

bool Window::CreateWnd()
{
	WNDCLASSEX wndclass;

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = Window::WinProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = WndIcon;
	wndclass.hCursor = WndCursor;
	wndclass.hbrBackground = (HBRUSH) CreateSolidBrush(WndColor);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = "GameWnd";
	wndclass.hIconSm = WndIcon;

	if (!RegisterClassEx(&wndclass))
	{
		return false;
	}

	hWND = CreateWindowEx(
		NULL,
		"GameWnd",
		WndTitle.c_str(),
		WndStyle,
		WndInitialPosX, WndInitialPosY,
		WndWidth, WndHeight,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (WndMode == WINDOWED)
	{
		RECT WndRect = {0, 0, WndWidth, WndHeight};

		AdjustWindowRectEx(&WndRect,
			GetWindowStyle(hWND),
			GetMenu(hWND) != NULL,
			GetWindowStyle(hWND));

		WndInitialPosX = GetSystemMetrics(SM_CXSCREEN) / 2 - (WndRect.right - WndRect.left) / 2;
		WndInitialPosY = GetSystemMetrics(SM_CYSCREEN) / 2 - (WndRect.bottom - WndRect.top) / 2;

		MoveWindow(hWND,
			WndInitialPosX,
			WndInitialPosY,
			WndRect.right - WndRect.left,
			WndRect.bottom - WndRect.top,
			TRUE
		);
	}

	return (hWND ? true : false);
}

LRESULT CALLBACK Window::WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		WndKeys[wParam] = true;
		return 0;
	case WM_KEYUP:
		WndKeys[wParam] = false;
		return 0;

	case WM_MOUSEMOVE:
		WndMouseXCoord = GET_X_LPARAM(lParam);
		WndMouseYCoord = GET_Y_LPARAM(lParam);
		return 0;

	case WM_LBUTTONDOWN:
		WndKeys[VK_LBUTTON] = true;
		return 0;
	case WM_MBUTTONDOWN:
		WndKeys[VK_MBUTTON] = true;
		return 0;
	case WM_RBUTTONDOWN:
		WndKeys[VK_RBUTTON] = true;
		return 0;

	case WM_LBUTTONUP:
		WndKeys[VK_LBUTTON] = false;
		return 0;
	case WM_MBUTTONUP:
		WndKeys[VK_MBUTTON] = false;
		return 0;
	case WM_RBUTTONUP:
		WndKeys[VK_RBUTTON] = false;
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

