#pragma once
#include <SimpleMath.h>
#include <memory>
#include "Structs.h"
#include "IResource.h"

using std::shared_ptr;


class Light
{
public:
	Light()
	{
		this->_lightBuffer = std::make_shared<LightBuffer>();
		this->Create(Vector4(0.65f, 0.65f, 0.65f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 
			Vector3(DirectX::XM_PI, 0.0, DirectX::XM_PI));
	}

	~Light()
	{
		LOG_INFO("Light Destruct!");
	}

	void Create(Vector4 ambientLight, Vector4 diffuseColor, Vector3 lightDirection)
	{
		this->_lightBuffer = std::make_shared<LightBuffer>();

		this->_lightBuffer->ambientLight = ambientLight;
		this->_lightBuffer->diffuseColor = diffuseColor;
		this->_lightBuffer->lightDirection = lightDirection;
		this->_lightBuffer->padding = 0.0f;
	}

	inline shared_ptr<LightBuffer> GetLightBuffer() const
	{
		return _lightBuffer;
	}

	void SetAmbientLight(Vector4 ambientLight) const
	{
		this->_lightBuffer->ambientLight = ambientLight;
	}

	void SetDiffuseColor(Vector4 diffuseColor) const
	{
		this->_lightBuffer->diffuseColor = diffuseColor;
	}

	void SetLightDirection(Vector3 lightDirection) const
	{
		this->_lightBuffer->lightDirection = lightDirection;
	}

private:
	shared_ptr<LightBuffer> _lightBuffer;
};