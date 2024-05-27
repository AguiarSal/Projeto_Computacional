#include "GameObject.h"
#include "Engine.h"

Window*& GameObject::window = Engine::window;
float& GameObject::gameTime = Engine::frameTime;

GameObject::GameObject()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.5f;
}

GameObject::~GameObject()
{

}