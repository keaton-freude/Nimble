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
	TextureArray(D3DDevice device, D3DDeviceContext deviceContext, std::vector<wstring> texture_paths, bool splatMap = false);
	~TextureArray();

	ID3D11ShaderResourceView** GetTextureViews();
	unsigned int GetNumberOfTextures();
	ID3D11ShaderResourceView* GetShaderResourceView(unsigned int index);
	ID3D11Resource* GetTexture(unsigned int index);
private:
	void Create(D3DDevice device, D3DDeviceContext deviceContext, vector<wstring> texture_paths);
	void CreateForSplat(D3DDevice device, D3DDeviceContext deviceContext, vector<wstring> texture_paths);

	unsigned int _numTextures;
	ID3D11ShaderResourceView* _textureViews[MAX_TEXTURES];
	ID3D11Resource* _textures[MAX_TEXTURES];
	SplatBuffer _splatBuffer;
};