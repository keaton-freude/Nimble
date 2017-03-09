#include "SplatMap.h"

SplatMap::SplatMap()
{
}

void SplatMap::LoadFromFile(D3DDevice device, D3DDeviceContext deviceContext, const vector<wstring>& textures)
{
	_textures = TextureArray(device, deviceContext, textures, true);
}

SplatMap::~SplatMap()
{
}

ID3D11ShaderResourceView** SplatMap::GetTextureViews()
{
	return _textures.GetTextureViews();
}

size_t SplatMap::GetNumberOfViews()
{
	return _textures.GetNumberOfTextures();
}

ID3D11Resource* SplatMap::GetSplat(int texture_index)
{
	// texture_index refers to the detail texture index
	
	// 0-3 -> splat0
	if (texture_index <= 3)
	{
		return _textures.GetTexture(0);
	}
	else if (texture_index <= 7)
	{
		return _textures.GetTexture(5);
	}
	else if (texture_index <= 11)
	{
		return _textures.GetTexture(10);
	}

	return _textures.GetTexture(0);
}

int SplatMap::GetColorComponent(int texture_index)
{
	// texture_index_0 -> SKIP (splat)
	// texture_index_1 -> r
	// texture_index_2 -> g
	// texture_index_3 -> b
	// texture_index_4 -> a
	// texture_idnex_5 -> SKIP (splat)
	// texture_index_6 -> r
	// ...

	// r = 0, g = 1, b = 2, a = 3
	switch (texture_index)
	{
	case 1:
		return 0;
	case 2:
		return 1;
	case 3:
		return 2;
	case 4:
		return 3;
	case 6:
		return 0;
	case 7:
		return 1;
	case 8:
		return 2;
	case 9:
		return 3;
	case 11:
		return 0;
	case 12:
		return 1;
	case 13:
		return 2;
	case 14:
		return 3;
	}

	return 0;
}


