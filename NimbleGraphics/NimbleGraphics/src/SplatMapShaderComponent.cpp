#include "SplatMapShaderComponent.h"

bool SplatMapShaderComponent::Apply(D3DDevice& device, D3DDeviceContext& deviceContext)
{
	deviceContext->PSSetConstantBuffers(0, 1, _splatBuffer.GetAddressOf());
	deviceContext->PSSetShaderResources(0, _numTextureViews, _splatTextureViews);

	return true;
}

bool SplatMapShaderComponent::Load(D3DDevice& device, D3DDeviceContext& deviceContext)
{
	D3D11_BUFFER_DESC splatBufferDesc;

	splatBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	splatBufferDesc.ByteWidth = sizeof(SplatBuffer);
	splatBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	splatBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	splatBufferDesc.MiscFlags = 0;
	splatBufferDesc.StructureByteStride = 0;

	auto hr = device->CreateBuffer(&splatBufferDesc, nullptr, _splatBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		LOG_ERROR("Failed to create splat buffer.");
		return false;
	}

	return true;
}
