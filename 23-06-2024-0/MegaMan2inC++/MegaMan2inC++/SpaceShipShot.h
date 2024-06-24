#ifndef SPACESHIPSHOT_H
#define SPACESHIPSHOT_H

#include "GameObject.h"
#include "Sprites.h"

class SpaceShipShot : public GameObject
{
private:
	Sprites* Sprite;
	float vely;
	float velx;

public:
	SpaceShipShot(Img* img);
	~SpaceShipShot();

	void ObjUpdate();
	void ObjDraw();

	void SetShotVelX(float vx);
};

inline void SpaceShipShot::ObjDraw() {
	Sprite->SprtDraw(x, y, z);
}

inline void SpaceShipShot::SetShotVelX(float vx) {
	velx = vx;
}

#endif
