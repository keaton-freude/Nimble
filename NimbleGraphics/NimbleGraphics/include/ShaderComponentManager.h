#pragma once
#include <vector>
#include "IShaderComponent.h"
#include "IShader.h"


class ShaderComponentManager
{
public:
	ShaderComponentManager();

	void Add(shared_ptr<IShaderComponent> component);
	void Load(D3DDevice device, D3DDeviceContext deviceContext);
	void Apply(D3DDevice device, D3DDeviceContext deviceContext);

	std::vector<shared_ptr<IShaderComponent>>& GetComponents()
	{
		return _components;
	}
private:
	std::vector<shared_ptr<IShaderComponent>> _components;
};
