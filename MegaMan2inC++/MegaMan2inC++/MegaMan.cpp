#include "MegaMan.h"

MegaMan::MegaMan()
{
	Sprite = new Sprites("Graphics/Sprites/MegaMan/MegaManIdle1.png");
	ObjMoveTo(window->GetWndCenterX() - Sprite->GetSprtWidth() / 2.0f, 500.0f, Layer::Layer1);
	CtrlLeft = true;
	CtrlRight = true;
}

MegaMan::~MegaMan()
{
	delete Sprite;
}

void MegaMan::ObjUpdate()
{
	if (window->KeyDown(VK_LEFT))
	{
		ObjTranslate(-2.0f, 0.0f);
	}
	
	if (window->KeyDown(VK_RIGHT))
	{
		ObjTranslate(2.0f, 0.0f);
	}

	if (x < 24)
	{
		ObjMoveTo(24, y);
	}

	if (x > 976)
	{
		ObjMoveTo(976, y);
	}
}