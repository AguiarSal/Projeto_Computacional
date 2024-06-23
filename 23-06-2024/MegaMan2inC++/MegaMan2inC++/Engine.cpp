#include "Engine.h"
#include <windows.h>
#include <sstream>
using std::stringstream;

Game* Engine::game = nullptr;
Window* Engine::window = nullptr;
Direct3D* Engine::direct3D = nullptr;
float Engine::frameTime = 0.0f;
bool Engine::paused = false;
FPS Engine::timer;
Renderer* Engine::renderer = nullptr;

Engine::Engine()
{
	window = new Window();
	direct3D = new Direct3D();
	renderer = new Renderer();
}

Engine::~Engine()
{
	delete game;
	delete window;
	delete direct3D;
	delete renderer;
}

int Engine::Start(Game* level)
{
	game = level;

	if (!window->CreateWnd())
	{
		MessageBox(nullptr, "Deu ruim aí", "Engine", MB_OK);
		return EXIT_FAILURE;
	}

	if (!direct3D->Initialize(window))
	{
		MessageBox(window->GetWndId(), "Dispositivo gráfico tá dando ruim", "Engine", MB_OK);
		return EXIT_FAILURE;
	}

	if (!renderer->Initialize(window, direct3D))
	{
		MessageBox(window->GetWndId(), "Renderizador tá dando ruim na inicialização", "Engine", MB_OK);
		return EXIT_FAILURE;
	}
	
	timeBeginPeriod(1);

	int exitCode = PrincipalLoop();

	timeEndPeriod(1);

	return exitCode;
}

void Engine::Pause()
{
	paused = true;
	timer.Stop();
}

void Engine::Resume()
{
	paused = false;
	timer.Start();
}

int Engine::PrincipalLoop()
{
	timer.Start();
	
	game->Initialization();

	MSG msg = { 0 };
	bool PauseKeyControl = true;

	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (PauseKeyControl)
			{
				if (window->KeyDown(VK_SHIFT))
				{
					paused = !paused;
					PauseKeyControl = false;

					if (paused)
					{
						timer.Stop();
					}
					else
					{
						timer.Start();
					}
				}
			}
			else
			{
				if (window->KeyUp(VK_SHIFT))
				{
					PauseKeyControl = true;
				}
			}

			if (!paused)
			{
				frameTime = FrameTime();

				game->Update();

				direct3D->Clear();

				game->Draw();

				renderer->Render();

				direct3D->Present();
			}
			else
			{
				game->Paused();
			}
		}
	} while (msg.message != WM_QUIT);

	game->Finalize();

	return int(msg.wParam);
}

float Engine::FrameTime()
{
#ifdef _DEBUG
	static float totalTime = 0.0f;
	static unsigned int frameCount = 0;
#endif

	frameTime = timer.Reset();

#ifdef _DEBUG
	totalTime += frameTime;
	frameCount++;

	if (totalTime >= 1.0f)
	{
		stringstream text;
		text << std::fixed;
		text.precision(3);

		text << window->GetWndTitle().c_str() << "  "
			<< "FPS: " << frameCount << "  "
			<< "Frame Time: " << frameTime * 1000 << " ms";

		SetWindowText(window->GetWndId(), text.str().c_str());

		frameCount = 0;
		totalTime -= 1.0f;
	}
#endif

	return frameTime;
}