#include "Spaceship.h"
#include "SpaceShipShot.h"
#include "SpaceInvaders.h"

SpaceShip::SpaceShip()
{
	Sprite = new Sprites("Graphics/Sprites/SpaceShip/SpaceShip.png");
	shot = new Img("Graphics/Sprites/SpaceShip/SpaceShipShot.png");


	ObjMoveTo(window->GetWndCenterX() - Sprite->GetSprtWidth() / 2.0f, window->GetWndHeigth() - 80.0f, Layer::Layer0);
	velx = 100;
	keyCtrl = true;
}

SpaceShip::~SpaceShip()
{
	delete Sprite;
	delete shot;
}

void SpaceShip::ObjUpdate()
{
	if (keyCtrl && window->KeyDown(VK_SPACE))
	{
		SpaceShipShot* shots = new SpaceShipShot(shot);
		shots->ObjMoveTo(x + Sprite->GetSprtWidth() / 2.0f - 2, y, Layer::Layer1);
		SpaceInvaders::scene->Add(shots);
		keyCtrl = false;
	}
	else if (window->KeyUp(VK_SPACE))
	{
		keyCtrl = true;
	}

	if (window->KeyDown(VK_RIGHT))
	{
		ObjTranslate(velx * gameTime, 0);
	}
	if (window->KeyDown(VK_LEFT))
	{
		ObjTranslate(-velx * gameTime, 0);
	}

	if (x < 0)
	{
		ObjMoveTo(0, y);
	}
	if (x + Sprite->GetSprtWidth() > window->GetWndWidth())
	{
		ObjMoveTo(window->GetWndWidth() - Sprite->GetSprtWidth(), y);
	}
}
