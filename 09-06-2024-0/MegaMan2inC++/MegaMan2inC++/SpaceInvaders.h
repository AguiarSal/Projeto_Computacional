#ifndef SPACEINVADERS_H
#define SPACEINVADERS_H

#include "Game.h"
#include "Scene.h"
#include "Sprites.h"

enum ObjTypes{SPACESHIP, SPACESHIPSHOTS};

class SpaceInvaders : public Game
{
private:
	Sprites* background = nullptr;
	
	bool viewScene = true;
	bool viewBBox = false;
	bool ctrlKeyS = true;
	bool ctrlKeyB = true;

public:
	static Scene* scene;

	void Initialization();
	void Update();
	void Draw();
	void Finalize();
};

#endif
