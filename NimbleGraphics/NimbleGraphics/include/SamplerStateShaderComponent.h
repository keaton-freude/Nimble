#pragma once
#include "IShaderComponent.h"
#include "Helper.h"

class SamplerStateShaderComponent : public IShaderComponent
{
public:
	SamplerStateShaderComponent()
		: IShaderComponent(), _samplerState(nullptr)
	{
		
	}

	SamplerStateShaderComponent(ID3D11SamplerState* state)
		: IShaderComponent(), _samplerState(state)
	{
		
	}


	bool Apply(D3DDevice& device, D3DDeviceContext& deviceContext) override;
	bool Load(D3DDevice& device, D3DDeviceContext& deviceContext) override;
private:
	ID3D11SamplerState* _samplerState;
	
};
