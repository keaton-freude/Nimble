#include "TextureArray.h"
#include <WICTextureLoader.h>
#include "Logger.h"

TextureArray::TextureArray()
	: _numTextures(0)
{
}

TextureArray::TextureArray(D3DDevice device, D3DDeviceContext deviceContext, std::vector<wstring> texture_paths)
	: _numTextures(texture_paths.size())
{
	unsigned int i = 0;

	for (const auto& s : texture_paths)
	{
		auto hr = DirectX::CreateWICTextureFromFile(device.Get(), deviceContext.Get(), s.c_str(), nullptr, &_textureViews[i++]);

		if (FAILED(hr))
		{
			LOG_ERROR("Could not load texture: ", s.c_str(), " -- Reason: ", hr);
		}
	}
}

TextureArray::~TextureArray()
{
}

ID3D11ShaderResourceView** TextureArray::GetTextureViews()
{
	return _textureViews;
}

unsigned TextureArray::GetNumberOfTextures()
{
	return _numTextures;
}
