#pragma once
#include "LightShaderComponent.h"
#include "Light.h"

bool LightShaderComponent::Apply(D3DDevice& device, D3DDeviceContext& deviceContext)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	result = deviceContext->Map(_lightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	if (FAILED(result))
	{
		LOG_ERROR("Failed to map light buffer.");
		return false;
	}

	auto dataPtr = static_cast<LightBuffer*>(mappedResource.pData);

	auto lightBuffer = _light.GetLightBuffer();
	
	dataPtr->ambientLight = lightBuffer.ambientLight;
	dataPtr->diffuseColor = lightBuffer.diffuseColor;
	dataPtr->lightDirection = lightBuffer.lightDirection;
	dataPtr->padding = 0.0f;

	deviceContext->Unmap(_lightBuffer.Get(), 0);

	deviceContext->PSSetConstantBuffers(0, 1, _lightBuffer.GetAddressOf());

	return true;
}

bool LightShaderComponent::Load(D3DDevice& device, D3DDeviceContext& deviceContext)
{
	D3D11_BUFFER_DESC lightBufferDesc;

	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBuffer);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	HRESULT result = device->CreateBuffer(&lightBufferDesc, nullptr, _lightBuffer.GetAddressOf());

	if (FAILED(result))
	{
		LOG_ERROR("Failed to create light buffer.");
		return false;
	}



	return true;
}
