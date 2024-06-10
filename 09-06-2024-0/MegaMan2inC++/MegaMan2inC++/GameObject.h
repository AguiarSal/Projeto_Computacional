#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Window.h"
#include "Game.h"
#include "Geom.h"

class GameObject
{
private:
	float posx, posy, posz;
	Geom* BoundingBox;

protected:
	static Window*& window;
	static Game*& game;
	static float& gameTime;

	const float& x = posx;
	const float& y = posy;
	const float& z = posz;

	unsigned int Type;

public:

	GameObject();
	virtual ~GameObject();
	
	virtual float GetObjX() const;
	virtual float GetObjY() const;
	virtual float GetObjZ() const;
	virtual unsigned int GetObjType() const;
	virtual Geom* GetObjBBox() const;

	virtual void BBox(Geom* Bb);
	virtual void ObjTranslate(float deltaX, float deltaY, float deltaZ = 0.0f);
	virtual void ObjMoveTo(float pX, float pY, float pZ);
	virtual void ObjMoveTo(float pX, float pY);
	virtual void OnCollision(GameObject* obj);

	virtual void ObjUpdate() = 0;
	virtual void ObjDraw() = 0;
};

inline float GameObject::GetObjX() const {
	return posx;
}

inline float GameObject::GetObjY() const {
	return posy;
}

inline float GameObject::GetObjZ() const {
	return posz;
}

inline unsigned int GameObject::GetObjType() const {
	return Type;
}

inline Geom* GameObject::GetObjBBox() const {
	return BoundingBox;
}

#endif
