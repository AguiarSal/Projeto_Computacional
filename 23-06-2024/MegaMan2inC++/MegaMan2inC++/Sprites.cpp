#include "Sprites.h"
#include "Engine.h"

const float Layer::Layer0 = 0.0f;
const float Layer::Layer1 = 0.5f;
const float Layer::Layer2 = 0.7f;

Sprites::Sprites(string file)
{
	img = new Img(file);
	localImg = true;

	Sprt.SprtTexture = img->GetImgView();
}

Sprites::Sprites(const Img* image)
{
	img = image;
	localImg = false;

	Sprt.SprtTexture = img->GetImgView();
}

Sprites::~Sprites()
{
	if (localImg)
	{
		delete img;
	}
}

void Sprites::SprtDraw(float x, float y, float z)
{
	Sprt.SprtX = x;
	Sprt.SprtY = y;
	Sprt.SprtScale = 1.0f;
	Sprt.SprtDepth = z;
	Sprt.SprtRotation = 0.0f;
	Sprt.SprtWidth = img->GetImgWidth();
	Sprt.SprtHeight = img->GetImgHeight();

	Engine::renderer->RenderDraw(&Sprt);
}