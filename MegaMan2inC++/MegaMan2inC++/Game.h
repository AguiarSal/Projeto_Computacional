#ifndef GAME_H
#define GAME_H

#include "Window.h"

class Game
{

protected:
	static Window* &window;

public:
	Game();
	virtual ~Game();

	virtual void Initialization() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Finalize() = 0;
};

#endif
