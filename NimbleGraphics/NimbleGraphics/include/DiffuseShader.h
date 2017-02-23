#pragma once
#include "BasicShader.h"
#include "Texture.h"


/*
 * A DiffuseShader is a BasicShader which also includes
 * 
 */
class DiffuseShader: public BasicShader
{
public:
	DiffuseShader(const D3DDevice& device, const D3DDeviceContext& deviceContext)
		: BasicShader(device, deviceContext)
	{
		this->_vsFilename = L"..\\Assets\\Shaders\\Diffuse.vs";
		this->_psFilename = L"..\\Assets\\Shaders\\Diffuse.ps";
		this->_vertexShaderEntryPoint = "vertexMain";
		this->_pixelShaderEntryPoint = "pixelMain";
		this->_vsVersion = "vs_5_0";
		this->_psVersion = "ps_5_0";
	}

	~DiffuseShader()
	{
		LOG_INFO("Diffuse Shader Destruct!");
	}

	void SetDiffuseTexture(shared_ptr<Texture> texture);

protected:
	void GetPolygonLayout(shared_ptr<D3D11_INPUT_ELEMENT_DESC>& desc, unsigned& numElements) override;
	void SetComponents() override;

private:
};
