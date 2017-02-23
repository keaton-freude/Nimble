#pragma once
#include "Typedefs.h"
#include <memory>
#include "Structs.h"

using std::shared_ptr;


class Light
{
public:
	Light();
	~Light();

	Light(Vector3 lightDirection, Vector4 diffuseColor, Vector4 ambientLight)
	{
		_lightBuffer.ambientLight = ambientLight;
		_lightBuffer.diffuseColor = diffuseColor;
		_lightBuffer.lightDirection = lightDirection;
	}

	const LightBuffer& GetLightBuffer() const;

	void SetAmbientLight(Vector4 ambientLight);
	void SetDiffuseColor(Vector4 diffuseColor);
	void SetLightDirection(Vector3 lightDirection);

private:
	void Create(Vector4 ambientLight, Vector4 diffuseColor, Vector3 lightDirection);

private:
	LightBuffer _lightBuffer;
};