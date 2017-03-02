#include "SamplerStateShaderComponent.h"

bool SamplerStateShaderComponent::Apply(D3DDevice& device, D3DDeviceContext& deviceContext)
{
	deviceContext->PSSetSamplers(0, 1, &_samplerState);

	return true;
}

bool SamplerStateShaderComponent::Load(D3DDevice& device, D3DDeviceContext& deviceContext)
{

	return true;
}
