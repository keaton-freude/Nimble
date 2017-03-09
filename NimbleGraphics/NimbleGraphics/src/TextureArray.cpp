#include "TextureArray.h"
#include <WICTextureLoader.h>
#include "Logger.h"

TextureArray::TextureArray()
	: _numTextures(0)
{
}

TextureArray::TextureArray(D3DDevice device, D3DDeviceContext deviceContext, const std::vector<wstring>& texture_paths, bool splatMap)
	: _numTextures(texture_paths.size())
{
	if (splatMap)
		CreateForSplat(device, deviceContext, texture_paths);
	else
		Create(device, deviceContext, texture_paths);
}

TextureArray::~TextureArray()
{
}

ID3D11ShaderResourceView** TextureArray::GetTextureViews()
{
	return _textureViews;
}

size_t TextureArray::GetNumberOfTextures()
{
	return _numTextures;
}

ID3D11ShaderResourceView* TextureArray::GetShaderResourceView(unsigned index)
{
	return _textureViews[index];
}

ID3D11Resource* TextureArray::GetTexture(unsigned index)
{
	return _textures[index];
}

void TextureArray::Create(D3DDevice device, D3DDeviceContext deviceContext, const vector<wstring>& texture_paths)
{
	unsigned int i = 0;

	for (const auto& s : texture_paths)
	{
		//auto hr = DirectX::CreateWICTextureFromFile(device.Get(), deviceContext.Get(), s.c_str(), &_textures[i], &_textureViews[i]);
		auto hr = DirectX::CreateWICTextureFromFileEx(device.Get(), deviceContext.Get(), s.c_str(), 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE,
			0, 0, false, &_textures[i], &_textureViews[i]);
		i++;
		if (FAILED(hr))
		{
			LOG_ERROR("Could not load texture: ", s.c_str(), " -- Reason: ", hr);
		}
	}
}

void TextureArray::CreateForSplat(D3DDevice device, D3DDeviceContext deviceContext, const vector<wstring>& texture_paths)
{
	// This version assumes that the 0th, 5th, 10th, 15th, etc image should be loaded as Dynamic, which means not
	// generating mip maps, which means not passing in deviceContext.

	unsigned int i = 0;

	for (const auto& s : texture_paths)
	{
		HRESULT hr = 0;
		
		if (i % 5 != 0)
			hr = DirectX::CreateWICTextureFromFileEx(device.Get(), deviceContext.Get(), s.c_str(), 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE,
			0, 0, false, &_textures[i], &_textureViews[i]);
		else
			hr = DirectX::CreateWICTextureFromFileEx(device.Get(), s.c_str(), 0, D3D11_USAGE_DYNAMIC, D3D11_BIND_SHADER_RESOURCE,
				D3D11_CPU_ACCESS_WRITE, 0, false, &_textures[i], &_textureViews[i]);

		i++;
		if (FAILED(hr))
		{
			LOG_ERROR("Could not load texture: ", s.c_str(), " -- Reason: ", hr);
		}
	}
}
