#ifndef SPRITES_H
#define SPRITES_H

#include "Img.h"

struct SpriteData
{
	float SprtX, SprtY;
	float SprtScale;
	float SprtDepth;
	float SprtRotation;
	unsigned int SprtWidth;
	unsigned int SprtHeight;
	ID3D11ShaderResourceView* SprtTexture;
};

struct Layer
{
	static const float Layer0;
	static const float Layer1;
	static const float Layer2;
};

class Sprites
{
private:
	SpriteData Sprt;
	bool localImg;
	const Img* img;

public:
	Sprites(string file);
	Sprites(const Img* image);
	~Sprites();

	int GetSprtWidth();
	int GetSprtHeight();

	void SprtDraw(float x, float y, float z = Layer::Layer1);
};

inline int Sprites::GetSprtWidth() {
	return img->GetImgWidth();
}

inline int Sprites::GetSprtHeight() {
	return img->GetImgHeight();
}

#endif
