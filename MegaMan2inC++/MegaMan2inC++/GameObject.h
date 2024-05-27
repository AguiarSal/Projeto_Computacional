#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Window.h"

class GameObject
{
public:
	static Window*& window;
	static float& gameTime;
	float x, y, z;

	GameObject();
	virtual ~GameObject();

	virtual void ObjTranslate(float deltaX, float deltaY, float deltaZ = 0.0f);
	virtual void ObjMoveTo(float posX, float posY, float posZ);
	virtual void ObjMoveTo(float posX, float posY);

	virtual void ObjUpdate() = 0;
	virtual void ObjDraw() = 0;
};

inline void GameObject::ObjTranslate(float deltaX, float deltaY, float deltaZ) {
	x += deltaX;
	y += deltaY;
	z += deltaZ;
}

inline void GameObject::ObjMoveTo(float posX, float posY, float posZ) {
	x = posX;
	y = posY;
	z = posZ;
}

inline void GameObject::ObjMoveTo(float posX, float posY) {
	x = posX;
	y = posY;
}

#endif
