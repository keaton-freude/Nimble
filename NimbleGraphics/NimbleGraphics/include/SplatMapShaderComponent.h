#pragma once
#include "IShaderComponent.h"
#include <vector>
#include "SplatMap.h"

class SplatMapShaderComponent: public IShaderComponent
{
public:
	SplatMapShaderComponent()
		: _splatTextureViews(nullptr), _numTextureViews(0)
	{
		
	}

	bool Apply(D3DDevice& device, D3DDeviceContext& deviceContext) override;
	bool Load(D3DDevice& device, D3DDeviceContext& deviceContext) override;
private:
	ID3D11ShaderResourceView** _splatTextureViews;
	unsigned int _numTextureViews;
	D3DBuffer _splatBuffer;
};
