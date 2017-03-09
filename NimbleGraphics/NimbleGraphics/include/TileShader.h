#pragma once
#include "BasicShader.h"
#include "Texture.h"

class TileShader : public BasicShader
{
public:
	TileShader() = default;

	TileShader(D3DDevice device, D3DDeviceContext deviceContext)
		: BasicShader(device, deviceContext)
	{
		this->_vsFilename = L"..\\Assets\\Shaders\\TileShader.vs";
		this->_psFilename = L"..\\Assets\\Shaders\\TileShader.ps";
		this->_vertexShaderEntryPoint = "vertexMain";
		this->_pixelShaderEntryPoint = "pixelMain";
		this->_vsVersion = "vs_5_0";
		this->_psVersion = "ps_5_0";
	}

	~TileShader()
	{
	}

	void SetDiffuseTexture(shared_ptr<Texture> texture);

protected:
	void SetComponents() override;
	void GetPolygonLayout(shared_ptr<D3D11_INPUT_ELEMENT_DESC>& desc, unsigned int& numElements) override;
	

private:
};
