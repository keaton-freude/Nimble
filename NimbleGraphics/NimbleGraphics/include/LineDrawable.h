#pragma once
#include <d3d11.h>
#include "IDrawable.h"
#include "ColorShader.h"

class LineDrawable : public IDrawable
{
public:
	LineDrawable();
	~LineDrawable();

	void SetLine(ComPtr<ID3D11Device> device, Vector3 p1, Vector3 p2);
	void Draw(ComPtr<ID3D11DeviceContext> deviceContext, shared_ptr<Matrix> viewMatrix, 
		shared_ptr<Matrix> projectionMatrix);
	
};