#ifndef ENEMIES_H
#define ENEMIES_H

#include "GameObject.h"
#include "Sprites.h"

class Alien : public GameObject
{
private:
	float velx;
	int life;

	Sprites* Sprite;

public:
	Alien();
	~Alien();

	void ObjUpdate();
	void ObjDraw();

};

inline void Alien::ObjDraw() {
	Sprite->SprtDraw(x, y, z);
}

#endif
