#ifndef SPACEINVADERS_H
#define SPACEINVADERS_H

#include "Game.h"
#include "Scene.h"
#include "Sprites.h"

class SpaceInvaders : public Game
{
private:
	Sprites* background = nullptr;
	

public:
	static Scene* scene;

	void Initialization();
	void Update();
	void Draw();
	void Finalize();
};

#endif
