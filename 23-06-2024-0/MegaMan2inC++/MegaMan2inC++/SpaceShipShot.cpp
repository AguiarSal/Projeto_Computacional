#include "SpaceShipShot.h"
#include "SpaceInvaders.h"

SpaceShipShot::SpaceShipShot(Img* img)
{
	Sprite = new Sprites(img);

	BBox(new Rect(0, 0, Sprite->GetSprtWidth(), Sprite->GetSprtHeight()));

	vely = 300;
	velx = 0;
	Type = SPACESHIPSHOTS;
}

SpaceShipShot::~SpaceShipShot()
{
	delete Sprite;
}

void SpaceShipShot::ObjUpdate()
{
	ObjTranslate(0, -vely * gameTime);
	ObjTranslate(-velx * gameTime, 0);

	if (y < 0)
	{
		SpaceInvaders::scene->Delete();
	}
	if (x < 0 || x > window->GetWndWidth())
	{
		SpaceInvaders::scene->Delete();
	}
}