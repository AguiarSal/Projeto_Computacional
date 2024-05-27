#ifndef MEGAMAN_H
#define MEGAMAN_H

#include "GameObject.h"
#include "Sprites.h"

class MegaMan : public GameObject
{
private:
	Sprites* Sprite;
	bool CtrlLeft;
	bool CtrlRight;

public:
	MegaMan();
	~MegaMan();

	void ObjUpdate();
	void ObjDraw();
};

inline void MegaMan::ObjDraw() {
	Sprite->SprtDraw(x, y, z);
}

#endif
