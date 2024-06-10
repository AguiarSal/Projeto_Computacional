#include "GameObject.h"
#include "Engine.h"

Window*& GameObject::window = Engine::window;
Game*& GameObject::game		= Engine::game;
float& GameObject::gameTime = Engine::frameTime;

GameObject::GameObject()
{
	posx = 0.0f;
	posy = 0.0f;
	posz = 0.5f;

	Type = UNDEFINED_TYPE;

	BoundingBox = nullptr;
}

GameObject::~GameObject()
{
	if (BoundingBox)
	{
		delete BoundingBox;
	}
}

void GameObject::BBox(Geom* Bb)
{
	if (BoundingBox)
	{
		delete BoundingBox;
	}

	BoundingBox = Bb;
	BoundingBox->GeomTranslate(x, y);
}

void GameObject::ObjTranslate(float deltaX, float deltaY, float deltaZ)
{
	posx += deltaX;
	posy += deltaY;
	posz += deltaZ;

	if (BoundingBox)
	{
		BoundingBox->GeomTranslate(deltaX, deltaY);
	}
}

void GameObject::ObjMoveTo(float pX, float pY, float pZ)
{
	posx = pX;
	posy = pY;
	posz = pZ;

	if (BoundingBox)
	{
		BoundingBox->GeomMoveTo(pX, pY);
	}
}

void GameObject::ObjMoveTo(float pX, float pY)
{
	posx = pX;
	posy = pY;

	if (BoundingBox)
	{
		BoundingBox->GeomMoveTo(pX, pY);
	}
}

void GameObject::OnCollision(GameObject* obj)
{

}

