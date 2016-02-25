#pragma once

#include <d3d11.h>
#include <memory>
#include <wrl\client.h>
#include "Structs.h"

using std::unique_ptr;
using std::string;
using Microsoft::WRL::ComPtr;

class Texture
{
private:


public:
	Texture(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, string textureName);
	~Texture();

	inline ComPtr<ID3D11ShaderResourceView> GetTexture() { return _textureView; }
private:
	bool LoadTarga(string filename, int& height, int& width);
	bool Init(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, string textureName);

private:
	unique_ptr<unsigned char> _targaImage;
	ComPtr<ID3D11Texture2D> _texture;
	ComPtr<ID3D11ShaderResourceView> _textureView;
};