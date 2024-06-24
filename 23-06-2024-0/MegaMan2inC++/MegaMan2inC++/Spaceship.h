#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "GameObject.h"
#include "Sprites.h"

enum CurrentPowerUp{NONE, TRIPLE};

class SpaceShip : public GameObject
{
private:
	Sprites* Sprite;
	float velx;
	float ShotDelay;
	float lastShotTime;
	bool ShotCtrl;
	Img* shot;
	

	Img* NonePowerUp;
	Img* TriplePowerUp;

	unsigned int powerup;

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

