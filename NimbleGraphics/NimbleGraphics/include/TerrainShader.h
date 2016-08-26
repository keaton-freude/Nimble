#pragma once

#include "IShader.h"
#include "Light.h"
#include <d3d11.h>
#include "Structs.h"
#include "Helper.h"
#include <memory>

using std::shared_ptr;
using Microsoft::WRL::ComPtr;

class TerrainShader : public IShader
{
public:
	TerrainShader()
		: IShader(), _lightBuffer(), _sampleState()
	{
		LOG_INFO("Terrain Shader Construct!");
		this->_vsFilename = L"..\\..\\Assets\\Shaders\\Terrain.vs";
		this->_psFilename = L"..\\..\\Assets\\Shaders\\Terrain.ps";
		this->_vertexShaderEntryPoint = "vertexMain";
		this->_pixelShaderEntryPoint = "TerrainPixelShader";
		this->_vsVersion = "vs_5_0";
		this->_psVersion = "ps_5_0";
	}

	TerrainShader(ComPtr<ID3D11Device> device)
		: IShader(), _lightBuffer(), _sampleState()
	{
		LOG_INFO("Terrain Shader overloaded C'tor!");
		this->_vsFilename = L"..\\..\\Assets\\Shaders\\Terrain.vs";
		this->_psFilename = L"..\\..\\Assets\\Shaders\\Terrain.ps";
		this->_vertexShaderEntryPoint = "vertexMain";
		this->_pixelShaderEntryPoint = "TerrainPixelShader";
		this->_vsVersion = "vs_5_0";
		this->_psVersion = "ps_5_0";

		this->Load(device);
	}

	~TerrainShader()
	{
		LOG_INFO("Terrain Shader Destruct!");
		IShader::~IShader();
	}

	// IResource Load
	bool Load(ComPtr<ID3D11Device> device) override
	{
		HRESULT result;
		D3D11_SAMPLER_DESC samplerDesc;
		D3D11_BUFFER_DESC lightBufferDesc;

		// Do basic load work
		if (!IShader::Load(device))
		{
			cout << "IShader failed to load correctly in TerrainShader. " << endl;
			return false;
		}

		// Add on Terrain-specific loading work

		// Create a texture sampler state description.
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// Create the texture sampler state.

		result = device->CreateSamplerState(&samplerDesc, _sampleState.GetAddressOf());
		if (FAILED(result))
		{
			LOG_ERROR("Failed to create sampler state.");
			return false;
		}

		lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		lightBufferDesc.ByteWidth = sizeof(LightBuffer);
		lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		lightBufferDesc.MiscFlags = 0;
		lightBufferDesc.StructureByteStride = 0;

		// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
		result = device->CreateBuffer(&lightBufferDesc, NULL, _lightBuffer.GetAddressOf());
		if (FAILED(result))
		{
			cout << "Failed to create light Buffer Description." << endl;
			return false;
		}

		return true;
	}

	// IShader Draw
	bool Draw(ComPtr<ID3D11DeviceContext> deviceContext, int indexCount, Matrix worldMatrix,
		shared_ptr<Matrix> viewMatrix, shared_ptr<Matrix> projectionMatrix, shared_ptr<Light> light, ComPtr<ID3D11ShaderResourceView> grass,
		ComPtr<ID3D11ShaderResourceView> slope, ComPtr<ID3D11ShaderResourceView> rock)
	{
		bool result;

		// set the matrices here
		this->_worldMatrix = worldMatrix;
		this->_viewMatrix = *viewMatrix;
		this->_projectionMatrix = *projectionMatrix;

		// Set the shader parameters that it will use for rendering.
		result = this->SetShaderParameters(deviceContext, light, grass, slope, rock);

		if (!result)
		{
			return false;
		}

		// Now render the prepared buffers with the shader.
		RenderShader(deviceContext, indexCount);

		return true;
	}

	void TerrainShader::GetPolygonLayout(shared_ptr<D3D11_INPUT_ELEMENT_DESC>& desc, unsigned int & numElements) override
	{
		numElements = 3;

		desc = shared_ptr<D3D11_INPUT_ELEMENT_DESC>(new D3D11_INPUT_ELEMENT_DESC[numElements]);

		desc.get()[0].SemanticName = "POSITION";
		desc.get()[0].SemanticIndex = 0;
		desc.get()[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		desc.get()[0].InputSlot = 0;
		desc.get()[0].AlignedByteOffset = 0;
		desc.get()[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.get()[0].InstanceDataStepRate = 0;

		desc.get()[1].SemanticName = "TEXCOORD";
		desc.get()[1].SemanticIndex = 0;
		desc.get()[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		desc.get()[1].InputSlot = 0;
		desc.get()[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		desc.get()[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.get()[1].InstanceDataStepRate = 0;

		desc.get()[2].SemanticName = "NORMAL";
		desc.get()[2].SemanticIndex = 0;
		desc.get()[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		desc.get()[2].InputSlot = 0;
		desc.get()[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		desc.get()[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.get()[2].InstanceDataStepRate = 0;
	}

	void RenderShader(ComPtr<ID3D11DeviceContext> deviceContext, int indexCount)
	{
		// Set the vertex input layout.
		deviceContext->IASetInputLayout(this->_layout.Get());

		// Set the vertex and pixel shaders that will be used to render this triangle.
		deviceContext->VSSetShader(this->_vertexShader.Get(), NULL, 0);
		deviceContext->PSSetShader(this->_pixelShader.Get(), NULL, 0);

		auto ss = StatesHelper::GetInstance().GetStates()->LinearWrap();
		deviceContext->PSSetSamplers(0, 1, &ss);

		//deviceContext->PSSetSamplers(0, 1, _sampleState.GetAddressOf());

		// Draw the triangle.
		deviceContext->DrawIndexed(indexCount, 0, 0);
	}

	bool SetShaderParameters(ComPtr<ID3D11DeviceContext> deviceContext, shared_ptr<Light> light, ComPtr<ID3D11ShaderResourceView> grass, ComPtr<ID3D11ShaderResourceView> slope,
		ComPtr<ID3D11ShaderResourceView> rock)
	{
		HRESULT result;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		unsigned int bufferNumber;
		LightBuffer* dataPtr;

		result = deviceContext->Map(_lightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
		{
			return false;
		}

		dataPtr = (LightBuffer*)mappedResource.pData;

		auto lightBuffer = light->GetLightBuffer();

		dataPtr->ambientLight = lightBuffer->ambientLight;
		dataPtr->diffuseColor = lightBuffer->diffuseColor;
		dataPtr->lightDirection = lightBuffer->lightDirection;
		dataPtr->padding = 0.0f;

		deviceContext->Unmap(_lightBuffer.Get(), 0);

		bufferNumber = 0;
		deviceContext->PSSetConstantBuffers(bufferNumber, 1, _lightBuffer.GetAddressOf());
		deviceContext->PSSetShaderResources(0, 1, grass.GetAddressOf());
		deviceContext->PSSetShaderResources(1, 1, slope.GetAddressOf());
		deviceContext->PSSetShaderResources(2, 1, rock.GetAddressOf());
		// Must let IShader do its Matrix Buffer copies!
		return IShader::SetShaderParameters(deviceContext);
	}
private:

	ComPtr<ID3D11Buffer> _lightBuffer;
	ComPtr<ID3D11SamplerState> _sampleState;
};