#include "Img.h"
#include "Direct3D.h"

Img::Img(string file)
{
	textureView = nullptr;
	ImgWidth = 0;
	ImgHeight = 0;

	D3D11CreateTextureFromFile(
		Direct3D::Device,
		Direct3D::Context,
		file.c_str(),
		nullptr,
		&textureView,
		ImgWidth,
		ImgHeight
	);

}

Img::~Img()
{
	if (textureView)
	{
		ID3D11Resource* resource = nullptr;
		textureView->GetResource(&resource);

		if (resource)
		{
			resource->Release();
			resource = nullptr;
		}
		textureView->Release();
		textureView = nullptr;
	}
}