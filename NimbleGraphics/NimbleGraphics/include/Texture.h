#pragma once
#include "Typedefs.h"
#include <memory>

struct TargaHeader
{
	unsigned char data1[12];
	unsigned short width;
	unsigned short height;
	unsigned char bpp;
	unsigned char data2;
};

using std::unique_ptr;
using std::string;

class Texture
{
public:
	Texture(D3DDevice device, D3DDeviceContext deviceContext, string textureName);

	~Texture();
	D3DShaderResourceView GetTexture() const;
private:
	bool LoadTarga(string filename, int& height, int& width);
	bool Init(D3DDevice device, D3DDeviceContext deviceContext, string textureName);

private:
	unique_ptr<unsigned char> _targaImage;
	D3DTexture2D _texture;
	D3DShaderResourceView _textureView;
};