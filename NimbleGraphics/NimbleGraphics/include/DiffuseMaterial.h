#pragma once
#include "Material.h"
#include "DiffuseShader.h"

class DiffuseMaterial: public Material
{
public:
	DiffuseMaterial();
	explicit DiffuseMaterial(D3DDevice device, D3DDeviceContext deviceContext, shared_ptr<Texture> texture);
	~DiffuseMaterial()
	{
		
	}

	void Apply(D3DDeviceContext deviceContext) override;


private:
	std::weak_ptr<DiffuseShader> _shader;
	shared_ptr<Texture> _texture;
};