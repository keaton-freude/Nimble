#pragma once
#include <vector>
#include "IShaderComponent.h"


class ShaderComponentManager
{
public:
	ShaderComponentManager();

	void Add(shared_ptr<IShaderComponent> component);

	void Load(D3DDevice device, D3DDeviceContext deviceContext);
	void Apply(D3DDevice device, D3DDeviceContext deviceContext);

	inline std::vector<shared_ptr<IShaderComponent>>& GetComponents()
	{
		return _components;
	}

private:
	std::vector<shared_ptr<IShaderComponent>> _components;
};
