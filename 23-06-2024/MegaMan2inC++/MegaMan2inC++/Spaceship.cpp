#include "Spaceship.h"
#include "SpaceShipShot.h"
#include "SpaceInvaders.h"

SpaceShip::SpaceShip()
{
	
	NonePowerUp = new Img("Graphics/Sprites/SpaceShip/SpaceShip_NonePowerUp.png");
	TriplePowerUp = new Img("Graphics/Sprites/SpaceShip/SpaceShip_TriplePowerUp.png");
	Sprite = new Sprites(NonePowerUp);

	shot = new Img("Graphics/Sprites/SpaceShip/SpaceShipShot(2).png");

	BBox(new Rect(0, 0, Sprite->GetSprtWidth(), Sprite->GetSprtHeight()));
	
	ObjMoveTo(window->GetWndCenterX() - Sprite->GetSprtWidth() / 2.0f, window->GetWndHeigth() - 80.0f, Layer::Layer1);
	velx = 100;
	lastShotTime = 9.0f;
	ShotDelay = 1.0f;
	ShotCtrl = true;
	powerup = NONE;
	Type = SPACESHIP; 
}

SpaceShip::~SpaceShip()
{
	delete Sprite;
	delete shot;
	delete NonePowerUp;
	delete TriplePowerUp;
}

void SpaceShip::ObjUpdate()
{
	switch (powerup)
	{
	case NONE:
		{
		Sprite = new Sprites(NonePowerUp);
		break;
		}
	case TRIPLE:
		{
		Sprite = new Sprites(TriplePowerUp);
		break;
		}
	}

	lastShotTime += gameTime;

	if (ShotCtrl && window->KeyDown(VK_SPACE) && lastShotTime > ShotDelay)
	{
		switch (powerup)
		{
		case NONE:
			{
			SpaceShipShot* shots = new SpaceShipShot(shot);
			shots->ObjMoveTo(x + Sprite->GetSprtWidth() / 2.0f - 1, y, Layer::Layer1);
			SpaceInvaders::scene->Add(shots, MOVING);
			break;
			}
		case TRIPLE:
			{
			SpaceShipShot* tripleshot[3];
			for (int i = 0; i < 3; i++)
			{
				tripleshot[i] = new SpaceShipShot(shot);
				float xi = (i - 1)* 4.0f;
				tripleshot[i]->ObjMoveTo(x + Sprite->GetSprtWidth() / 2.0f - 1 + xi, y, Layer::Layer1);

				if (i == 0)
				{
					tripleshot[i]->SetShotVelX(30);
				}
				else if (i == 2)
				{
					tripleshot[i]->SetShotVelX(-30);
				}

				SpaceInvaders::scene->Add(tripleshot[i], MOVING);
			}
			break;
			}
		}
		lastShotTime = 0.0f;
		ShotCtrl = false;
	}
	else if (window->KeyUp(VK_SPACE))
	{
		ShotCtrl = true;
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
