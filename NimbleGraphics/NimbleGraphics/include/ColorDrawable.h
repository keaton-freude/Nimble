#pragma once

#include <d3d11.h>
#include "IDrawable.h"
#include "ColorShader.h"


class ColorDrawable : public IDrawable
{
public:
	ColorDrawable();
	~ColorDrawable();
	
	void Draw(ComPtr<ID3D11DeviceContext> deviceContext, shared_ptr<Matrix> viewMatrix, 
		shared_ptr<Matrix> projectionMatrix);
};