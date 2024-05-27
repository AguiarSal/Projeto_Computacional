#include "Graphics.h"
#include "Engine.h"
#include "Game.h"
#include "Img.h"
#include "Sprites.h"
#include "Graphics.h"
#include "MegaMan.h"

class MegaMan2 : public Game
{
private:
	Sprites* background = nullptr;
	MegaMan* megaman = nullptr;

public:
	void Initialization();
	void Update();
	void Draw();
	void Finalize();
};

void MegaMan2::Initialization()
{
	background = new Sprites("Graphics/Sprites/Backgrounds/Air Man Stage.png");
	megaman = new MegaMan();
}

void MegaMan2::Update()
{
	if (window->KeyDown(VK_ESCAPE))
	{
		window->CloseWnd();
	}

	megaman->ObjUpdate();
}

void MegaMan2::Draw()
{
	background->SprtDraw(0.0f, 0.0f, Layer::Layer2);

	megaman->ObjDraw();
}

void MegaMan2::Finalize()
{
	delete background;
	delete megaman;
}

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
					 _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	Engine* engine = new Engine();
	engine->window->SetWndMode(WINDOWED);
	engine->window->SetWndSize(1080, 720);
	engine->window->SetWndColor(0, 112, 187);
	engine->window->SetWndTitle("MegaMan2inC++");
	engine->window->SetWndIcon(IDI_MEGAICON);
	engine->window->SetWndCursor(IDC_MEGACURSOR);

	engine->direct3D->ActiveVSync(true);
	
	int exit_code = engine->Start(new MegaMan2());

	delete engine;
	return exit_code;
}