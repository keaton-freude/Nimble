#pragma once
#include "IShaderComponent.h"
#include "Light.h"


class LightShaderComponent: public IShaderComponent
{
public:
	LightShaderComponent()
		: IShaderComponent()
	{
		
	}

	void SetLight(Light light)
	{
		_light = light;
	}

	bool Apply(D3DDevice& device, D3DDeviceContext& deviceContext) override;
	bool Load(D3DDevice& device, D3DDeviceContext& deviceContext) override;

private:
	Light _light;
	D3DBuffer _lightBuffer;
};
