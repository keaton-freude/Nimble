#pragma once
#include "BasicShader.h"
#include "SplatMap.h"

class TileShader : BasicShader
{
public:
	TileShader(const D3DDevice& device, const D3DDeviceContext& deviceContext)
		: BasicShader(device, deviceContext)
	{
		this->_vsFilename = L"..\\Assets\\Shaders\\TileShader.vs";
		this->_psFilename = L"..\\Assets\\Shaders\\TileShader.ps";
		this->_vertexShaderEntryPoint = "vertexMain";
		this->_pixelShaderEntryPoint = "pixelMain";
		this->_vsVersion = "vs_5_0";
		this->_psVersion = "ps_5_0";
	}

protected:
	void SetComponents() override;
	void GetPolygonLayout(shared_ptr<D3D11_INPUT_ELEMENT_DESC>& desc, unsigned& numElements) override;
	

private:
};
