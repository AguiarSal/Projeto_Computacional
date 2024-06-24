#ifndef ENGINE_H
#define ENGINE_H

#include "Game.h"
#include "Window.h"
#include "Direct3D.h"
#include "FPS.h"
#include "Renderer.h"

class Engine
{
private:
	static FPS timer;
	static bool paused;
	
	float FrameTime();

	int PrincipalLoop();

public:
	static Game* game;
	static Window* window;
	static Direct3D* direct3D;
	static Renderer* renderer;
	static float frameTime;

	Engine();
	~Engine();

	int Start(Game* level);

	static void Pause();
	static void Resume();
};

#endif
