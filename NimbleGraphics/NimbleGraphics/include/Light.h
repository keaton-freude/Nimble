#pragma once
#include <SimpleMath.h>
#include <memory>
#include "Structs.h"
#include "IResource.h"

using std::shared_ptr;


class Light : public IResource
{
public:
	Light();
	~Light();

	bool Load(ComPtr<ID3D11Device> device) override;
	void Unload() override;

	void Create(Vector4 ambientLight, Vector4 diffuseColor, Vector3 lightDirection);

	inline shared_ptr<LightBuffer> GetLightBuffer() const
	{
		return _lightBuffer;
	}

	void SetAmbientLight(Vector4 ambientLight);
	void SetDiffuseColor(Vector4 diffuseColor);
	void SetLightDirection(Vector3 lightDirection);
	

private:
	shared_ptr<LightBuffer> _lightBuffer;
};