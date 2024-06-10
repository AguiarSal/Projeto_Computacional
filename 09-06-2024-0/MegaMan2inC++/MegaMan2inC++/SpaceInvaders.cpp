#include "Engine.h"
#include "Graphics.h"
#include "SpaceInvaders.h"
#include "Spaceship.h"

Scene* SpaceInvaders::scene = nullptr;

void SpaceInvaders::Initialization()
{
	scene = new Scene();

	background = new Sprites("Graphics/Sprites/Backgrounds/Black_Background.png");
	
	SpaceShip* spaceship = new SpaceShip();
	scene->Add(spaceship, MOVING);
	
}

void SpaceInvaders::Update()
{
	if (window->KeyDown(VK_ESCAPE))
	{
		window->CloseWnd();
	}

	if (ctrlKeyS && window->KeyDown('S'))
	{
		viewScene = !viewScene;
		ctrlKeyS = false;
	}
	else if (window->KeyUp('S'))
	{
		ctrlKeyS = true;
	}

	if (ctrlKeyB && window->KeyDown('B'))
	{
		viewBBox = !viewBBox;
		ctrlKeyB = false;
	}
	else if (window->KeyUp('B'))
	{
		ctrlKeyB = true;
	}

	scene->SceneUpdate();

	scene->CollisionDetection();
}

void SpaceInvaders::Draw()
{
	if (viewScene)
	{
		background->SprtDraw(0.0f, 0.0f, Layer::Layer2);
		scene->SceneDraw();
	}

	if (viewBBox)
	{
		scene->DrawBBox();
	}
}

void SpaceInvaders::Finalize()
{
	delete background;
	
	delete scene;
}

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
					 _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	Engine* engine = new Engine();
	engine->window->SetWndMode(WINDOWED);
	engine->window->SetWndSize(448, 512);
	engine->window->SetWndColor(0, 0, 0);
	engine->window->SetWndTitle("SpaceInvaders");
	engine->window->SetWndIcon(IDI_MEGAICON);
	engine->window->SetWndCursor(IDC_MEGACURSOR);

	engine->direct3D->ActiveVSync(true);
	
	int exit_code = engine->Start(new SpaceInvaders());

	delete engine;
	return exit_code;
}