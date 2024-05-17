#include "Graphics.h"
#include "Engine.h"
#include "Game.h"

class MegaMan2 : public Game
{
private:

public:
	void Initialization();
	void Update();
	void Draw();
	void Finalize();
};

void MegaMan2::Initialization()
{

}

void MegaMan2::Update()
{
	if (window->KeyDown(VK_ESCAPE))
	{
		window->CloseWnd();
	}
}

void MegaMan2::Draw()
{

}

void MegaMan2::Finalize()
{

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

	int exit_code = engine->Start(new MegaMan2());

	delete engine;
	return exit_code;
}