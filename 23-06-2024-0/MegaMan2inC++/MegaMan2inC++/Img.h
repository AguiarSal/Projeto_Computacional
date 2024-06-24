#ifndef IMG_H
#define IMG_H

#include "Texture.h"
#include <string>
using std::string;

class Img
{
private:
	ID3D11ShaderResourceView* textureView;
	unsigned int ImgWidth;
	unsigned int ImgHeight;

public:
	Img(string file);
	~Img();

	unsigned int GetImgWidth() const;
	unsigned int GetImgHeight() const;
	ID3D11ShaderResourceView* GetImgView() const;

};

inline unsigned int Img::GetImgWidth() const {
	return ImgWidth;
}

inline unsigned int Img::GetImgHeight() const {
	return ImgHeight;
}

inline ID3D11ShaderResourceView* Img::GetImgView() const {
	return textureView;
}


#endif
