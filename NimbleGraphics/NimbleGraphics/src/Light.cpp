#include "Light.h"

Light::Light(): _lightBuffer()
{
	this->Create(Vector4(0.20f, 0.20f, 0.20f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	             Vector3(DirectX::XM_PI, 0.0, 0.0));
}

Light::~Light()
{
	LOG_INFO("Light Destruct!");
}

const LightBuffer& Light::GetLightBuffer() const
{
	return _lightBuffer;
}

void Light::SetAmbientLight(Vector4 ambientLight)
{
	_lightBuffer.ambientLight = ambientLight;
}

void Light::SetDiffuseColor(Vector4 diffuseColor)
{
	_lightBuffer.diffuseColor = diffuseColor;
}

void Light::SetLightDirection(Vector3 lightDirection)
{
	_lightBuffer.lightDirection = lightDirection;
}

void Light::Create(Vector4 ambientLight, Vector4 diffuseColor, Vector3 lightDirection)
{
	_lightBuffer = LightBuffer();

	_lightBuffer.ambientLight = ambientLight;
	_lightBuffer.diffuseColor = diffuseColor;
	_lightBuffer.lightDirection = lightDirection;
	_lightBuffer.padding = 0.0f;
}
