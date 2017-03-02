#pragma once
#include "TextureArray.h"

// Wraps a TextureArray and provides helpful
// functionality for mapping textures with their
// splat maps.

// A splatmap, in our case, is a 4-channel 8-bit texture
// the four channels, rgba, are each linked to a texture,
// called a detail texture. r -> detail_texture_0, g-> detail_texture_1
// etc. 

// The textures are stored in a contiguous list like such:
// [0] = SplatMap_0
// [1] = DetailTexture1
// [2] = DetailTexture2
// [3] = DetailTexture3
// [4] = DetailTexture4
// [5] = SplatMap_1
// [6] = DetailTexture5
// and so on.
class SplatMap
{
public:
	SplatMap();
	void LoadFromFile(D3DDevice device, D3DDeviceContext deviceContext, const vector<wstring>& textures);
	~SplatMap();

	ID3D11ShaderResourceView** GetTextureViews();
	int GetNumberOfViews();
	ID3D11Resource* GetSplat(int texture_index);
	int GetColorComponent(int texture_index);

private:
	TextureArray _textures;
};