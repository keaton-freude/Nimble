#pragma once
#include "WVPShaderComponent.h"
#include "Logger.h"
#include "Structs.h"

bool WVPShaderComponent::Load(D3DDevice& device, D3DDeviceContext& deviceContext)
{
	HRESULT result;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	D3D11_BUFFER_DESC matrixBufferDesc = this->GetMatrixBufferDescription();

	// Create the constant buffer pointer so we can access the vertex shader constant buffer
	// from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, _matrixBuffer.GetAddressOf());
	if (FAILED(result))
	{
		LOG_ERROR("Failed to create Matrix Buffer in IShader");
		return false;
	}

	LOG_INFO("Created Matrix Buffer successfully.");

	return true;
}

bool WVPShaderComponent::Apply(D3DDevice& device, D3DDeviceContext& deviceContext)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	result = deviceContext->Map(this->_matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	if (FAILED(result))
	{
		return false;
	}

	MatrixBuffer* dataPtr;

	dataPtr = static_cast<MatrixBuffer*>(mappedResource.pData);

	// Copy the matrices into the constant buffer.
	dataPtr->world = _worldMatrix.Transpose();
	dataPtr->view = _viewMatrix.Transpose();
	dataPtr->projection = _projectionMatrix.Transpose();

	deviceContext->Unmap(this->_matrixBuffer.Get(), 0);
	deviceContext->VSSetConstantBuffers(0, 1, _matrixBuffer.GetAddressOf());

	return true;
}

D3D11_BUFFER_DESC WVPShaderComponent::GetMatrixBufferDescription()
{
	D3D11_BUFFER_DESC matrixBufferDesc;

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	return matrixBufferDesc;
}
