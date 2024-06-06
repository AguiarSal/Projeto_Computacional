#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "GameObject.h"
#include "Sprites.h"

class SpaceShip : public GameObject
{
private:
	Sprites* Sprite;
	float velx;
	bool keyCtrl;
	Img* shot;

public:
	SpaceShip();
	~SpaceShip();

	void ObjUpdate();
	void ObjDraw();

};

inline void SpaceShip::ObjDraw(){
	Sprite->SprtDraw(x, y, z);
}

#endif

