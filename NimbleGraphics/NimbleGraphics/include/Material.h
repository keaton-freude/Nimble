#pragma once

#include "ShaderManager.h"

using std::string;

class Material
{
public:
	Material();
	Material(shared_ptr<IShader> shader);
	virtual ~Material();

	IShader* GetShader();


	virtual void Apply(D3DDeviceContext deviceContext);
protected:
	shared_ptr<IShader> shader;
};