#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
#include <memory>
#include "Structs.h"

using std::shared_ptr;


class Light
{
private:
	LightBuffer _lightBuffer;

public:
	Light()
		: _lightBuffer()
	{
		this->Create(Vector4(0.65f, 0.65f, 0.65f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 
			Vector3(DirectX::XM_PI, 0.0, 0.0));
	}

	~Light()
	{
		LOG_INFO("Light Destruct!");
	}


	const LightBuffer& GetLightBuffer() const
	{
		return _lightBuffer;
	}

	void SetAmbientLight(Vector4 ambientLight)
	{
		_lightBuffer.ambientLight = ambientLight;
	}

	void SetDiffuseColor(Vector4 diffuseColor)
	{
		_lightBuffer.diffuseColor = diffuseColor;
	}

	void SetLightDirection(Vector3 lightDirection)
	{
		_lightBuffer.lightDirection = lightDirection;
	}

private:
	void Create(Vector4 ambientLight, Vector4 diffuseColor, Vector3 lightDirection)
	{
		_lightBuffer = LightBuffer();

		_lightBuffer.ambientLight = ambientLight;
		_lightBuffer.diffuseColor = diffuseColor;
		_lightBuffer.lightDirection = lightDirection;
		_lightBuffer.padding = 0.0f;
	}
};