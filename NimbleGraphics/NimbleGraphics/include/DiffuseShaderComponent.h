#pragma once
#include "IShaderComponent.h"
#include "Typedefs.h"
#include <memory>
#include "Texture.h"

class DiffuseShaderComponent: public IShaderComponent
{
public:
	DiffuseShaderComponent()
		: IShaderComponent()
	{
		
	}

	DiffuseShaderComponent(shared_ptr<Texture> texture)
	{
		SetTexture(texture);
	}


	bool Apply(D3DDevice& device, D3DDeviceContext& deviceContext) override;
	bool Load(D3DDevice& device, D3DDeviceContext& deviceContext) override;

	void SetTexture(shared_ptr<Texture> texture)
	{
		_texture = texture;
	}

private:
	std::shared_ptr<Texture> _texture;
};
