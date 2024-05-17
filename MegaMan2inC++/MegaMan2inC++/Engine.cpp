#include "Engine.h"
#include <windows.h>
#include <sstream>
using std::stringstream;

Game* Engine::game = nullptr;
Window* Engine::window = nullptr;
Direct3D* Engine::direct3D = nullptr;

Engine::Engine()
{
	window = new Window();
	direct3D = new Direct3D();
}

Engine::~Engine()
{
	delete game;
	delete window;
	delete direct3D;
}

int Engine::Start(Game* level)
{
	game = level;

	window->CreateWnd();

	direct3D->Initialize(window);

	return PrincipalLoop();
}

int Engine::PrincipalLoop()
{
	MSG msg = { 0 };
	HDC hdc;
	RECT rect;

	hdc = GetDC(window->GetWndId());

	GetClientRect(window->GetWndId(), &rect);

	game->Initialization();

	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			game->Update();

			direct3D->Clear();

			FillRect(hdc, &rect, CreateSolidBrush(window->GetWndColor()));

			game->Draw();

			direct3D->Present();

			Sleep(16.666);
		}
	} while (msg.message != WM_QUIT);

	game->Finalize();

	ReleaseDC(window->GetWndId(), hdc);

	return int(msg.wParam);
}