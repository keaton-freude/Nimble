#include "Light.h"
#include "Logger.h"
#include <SimpleMath.h>

using DirectX::XM_PI;

Light::Light()
{
	this->_lightBuffer = shared_ptr<LightBuffer>(new LightBuffer());
	this->Create(Vector4(0.65f, 0.65f, 0.65f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector3(XM_PI, 0.0, XM_PI));
}

Light::~Light()
{
	LOG_INFO("Light Destruct!");
}

void Light::Create(Vector4 ambientLight, Vector4 diffuseColor, Vector3 lightDirection)
{
	this->_lightBuffer = shared_ptr<LightBuffer>(new LightBuffer());

	this->_lightBuffer->ambientLight = ambientLight;
	this->_lightBuffer->diffuseColor = diffuseColor;
	this->_lightBuffer->lightDirection = lightDirection;
	this->_lightBuffer->padding = 0.0f;
}

bool Light::Load(ComPtr<ID3D11Device> device)
{
	return true;
}

void Light::Unload()
{
}

void Light::SetAmbientLight(Vector4 ambientLight)
{
	this->_lightBuffer->ambientLight = ambientLight;
}

void Light::SetDiffuseColor(Vector4 diffuseColor)
{
	this->_lightBuffer->diffuseColor = diffuseColor;
}

void Light::SetLightDirection(Vector3 lightDirection)
{
	this->_lightBuffer->lightDirection = lightDirection;
}