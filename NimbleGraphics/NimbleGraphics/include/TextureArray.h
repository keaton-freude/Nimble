#pragma once
#define MAX_TEXTURES 32

#include "Typedefs.h"
#include <vector>
#include <string>
#include "Structs.h"
using std::vector;
using std::wstring;

class TextureArray
{
public:
	TextureArray();
	TextureArray(D3DDevice device, D3DDeviceContext deviceContext, std::vector<wstring> texture_paths);
	~TextureArray();

	ID3D11ShaderResourceView** GetTextureViews();
	unsigned int GetNumberOfTextures();
private:
	unsigned int _numTextures;
	ID3D11ShaderResourceView* _textureViews[MAX_TEXTURES];
	SplatBuffer _splatBuffer;
};