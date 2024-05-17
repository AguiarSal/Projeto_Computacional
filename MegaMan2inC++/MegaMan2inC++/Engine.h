#ifndef ENGINE_H
#define ENGINE_H

#include "Game.h"
#include "Window.h"
#include "Direct3D.h"

class Engine
{
private:
	int PrincipalLoop();

public:
	static Game* game;
	static Window* window;
	static Direct3D* direct3D;

	Engine();
	~Engine();

	int Start(Game* level);
};

#endif
