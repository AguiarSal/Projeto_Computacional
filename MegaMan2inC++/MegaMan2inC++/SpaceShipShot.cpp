#include "SpaceShipShot.h"
#include "SpaceInvaders.h"

SpaceShipShot::SpaceShipShot(Img* img)
{
	Sprite = new Sprites(img);
	vely = 300;
}

SpaceShipShot::~SpaceShipShot()
{
	delete Sprite;
}

void SpaceShipShot::ObjUpdate()
{
	ObjTranslate(0, -vely * gameTime);

	if (y < 0)
	{
		SpaceInvaders::scene->Remove();
	}
}