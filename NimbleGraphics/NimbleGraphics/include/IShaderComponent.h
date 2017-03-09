#pragma once
#include "Typedefs.h"

class IShaderComponent
{
public:
	virtual ~IShaderComponent() {}
	
	virtual bool Apply(D3DDevice& device, D3DDeviceContext& deviceContext) = 0;

	virtual bool Load(D3DDevice& device, D3DDeviceContext& deviceContext) = 0;
};