#pragma once
#include "IShaderComponent.h"
#include "Helper.h"

/*
 * This component will load 
 */
class WVPShaderComponent: public IShaderComponent
{
public:
	WVPShaderComponent()
		: IShaderComponent(), _matrixBuffer(nullptr), _worldMatrix(), _viewMatrix(), _projectionMatrix()
	{
	}

	void SetMatrices(MatrixRef worldMatrix, MatrixRef viewMatrix, MatrixRef projectionMatrix)
	{
		_worldMatrix = worldMatrix;
		_viewMatrix = viewMatrix;
		_projectionMatrix = projectionMatrix;
	}

	bool Apply(D3DDevice& device, D3DDeviceContext& deviceContext) override;
	bool Load(D3DDevice& device, D3DDeviceContext& deviceContext) override;

private:
	D3D11_BUFFER_DESC GetMatrixBufferDescription();

private:
	D3DBuffer _matrixBuffer;

	Matrix _worldMatrix;
	Matrix _viewMatrix;
	Matrix _projectionMatrix;
};