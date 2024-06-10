#include "SpaceShipShot.h"
#include "SpaceInvaders.h"

SpaceShipShot::SpaceShipShot(Img* img)
{
	Sprite = new Sprites(img);

	BBox(new Rect(0, 0, Sprite->GetSprtWidth(), Sprite->GetSprtHeight()));

	vely = 300;

	Type = SPACESHIPSHOTS;
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
		SpaceInvaders::scene->Delete();
	}
}