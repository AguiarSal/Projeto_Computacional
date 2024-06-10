#ifndef SPACESHIPSHOT_H
#define SPACESHIPSHOT_H

#include "GameObject.h"
#include "Sprites.h"

class SpaceShipShot : public GameObject
{
private:
	Sprites* Sprite;
	float vely;

public:
	SpaceShipShot(Img* img);
	~SpaceShipShot();

	void ObjUpdate();
	void ObjDraw();

};

inline void SpaceShipShot::ObjDraw() {
	Sprite->SprtDraw(x, y, z);
}

#endif
