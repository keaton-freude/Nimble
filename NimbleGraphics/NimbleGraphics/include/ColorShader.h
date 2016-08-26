#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include <wrl\client.h>
#include <memory>

#include "IShader.h"

using namespace std;
using Microsoft::WRL::ComPtr;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Color;
using std::shared_ptr;


class ColorShader : public IShader
{
public:
	ColorShader()
		: IShader()
	{
		this->_vsFilename = L"..\\..\\Assets\\Shaders\\Color.vs";
		this->_psFilename = L"..\\..\\Assets\\Shaders\\Color.ps";
		this->_vertexShaderEntryPoint = "ColorVertexShader";
		this->_pixelShaderEntryPoint = "ColorPixelShader";
		this->_vsVersion = "vs_5_0";
		this->_psVersion = "ps_5_0";
	}

	explicit ColorShader(ComPtr<ID3D11Device> device)
		: IShader()
	{
		this->_vsFilename = L"..\\..\\Assets\\Shaders\\Color.vs";
		this->_psFilename = L"..\\..\\Assets\\Shaders\\Color.ps";
		this->_vertexShaderEntryPoint = "ColorVertexShader";
		this->_pixelShaderEntryPoint = "ColorPixelShader";
		this->_vsVersion = "vs_5_0";
		this->_psVersion = "ps_5_0";

		ColorShader::Load(device);
	}

	~ColorShader()
	{
		LOG_INFO("Color Shader Destruct!");
	}


	bool Load(ComPtr<ID3D11Device> device) override
	{
		return IShader::Load(device);
	}

	bool Draw(ComPtr<ID3D11DeviceContext> deviceContext, int indexCount, Matrix world, shared_ptr<Matrix> view,
		shared_ptr<Matrix> projection)
	{
		bool result;

		// set the matrices here
		this->_worldMatrix = world;
		this->_viewMatrix = *view;
		this->_projectionMatrix = *projection;

		// Set the shader parameters that it will use for rendering.
		result = this->SetShaderParameters(deviceContext);

		if (!result)
		{
			return false;
		}

		// Now render the prepared buffers with the shader.
		RenderShader(deviceContext, indexCount);

		return true;
	}

	ComPtr<ID3D11VertexShader> GetVertexShader()
	{
		return this->_vertexShader;
	}

private:
	void GetPolygonLayout(shared_ptr<D3D11_INPUT_ELEMENT_DESC>& desc,
		unsigned int & numElements) override
	{
		numElements = 2;

		desc = shared_ptr<D3D11_INPUT_ELEMENT_DESC>(new D3D11_INPUT_ELEMENT_DESC[2]);

		desc.get()[0].SemanticName = "POSITION";
		desc.get()[0].SemanticIndex = 0;
		desc.get()[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		desc.get()[0].InputSlot = 0;
		desc.get()[0].AlignedByteOffset = 0;
		desc.get()[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.get()[0].InstanceDataStepRate = 0;

		desc.get()[1].SemanticName = "COLOR";
		desc.get()[1].SemanticIndex = 0;
		desc.get()[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.get()[1].InputSlot = 0;
		desc.get()[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		desc.get()[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.get()[1].InstanceDataStepRate = 0;
	}

	void RenderShader(ComPtr<ID3D11DeviceContext> deviceContext, int indexCount)
	{
		// Set the vertex input layout.
		deviceContext->IASetInputLayout(this->_layout.Get());

		// Set the vertex and pixel shaders that will be used to render this triangle.
		deviceContext->VSSetShader(this->_vertexShader.Get(), NULL, 0);
		deviceContext->PSSetShader(this->_pixelShader.Get(), NULL, 0);

		// Draw the triangle.
		deviceContext->DrawIndexed(indexCount, 0, 0);
	}
};