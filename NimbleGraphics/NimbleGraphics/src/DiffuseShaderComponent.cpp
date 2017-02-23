#include "DiffuseShaderComponent.h"
#include "BasicShader.h"

bool DiffuseShaderComponent::Apply(D3DDevice& device, D3DDeviceContext& deviceContext)
{
	deviceContext->PSSetShaderResources(0, 1, _texture->GetTexture().GetAddressOf());

	return true;
}

bool DiffuseShaderComponent::Load(D3DDevice& device, D3DDeviceContext& deviceContext)
{
	return true;
}
