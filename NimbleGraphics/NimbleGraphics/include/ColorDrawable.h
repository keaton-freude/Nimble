#pragma once

#include <d3d11.h>
#include "IDrawable.h"
#include "ColorShader.h"
#include "ShaderManager.h"

class ColorDrawable : public IDrawable
{
public:
	ColorDrawable()
	{
	}

	~ColorDrawable()
	{
		LOG_INFO("Color Drawable destruct!");
	}
	
	void Draw(ComPtr<ID3D11DeviceContext> deviceContext, MatrixRef viewMatrix,
		MatrixRef projectionMatrix)
	{
		// bind our vertex buffer
		unsigned int stride;
		unsigned int offset;

		// Set vertex buffer stride and offset.
		stride = sizeof(TerrainVertex);
		offset = 0;

		// Set the vertex buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetVertexBuffers(0, 1, vertexBuffer->GetVertexBuffer().GetAddressOf(), &stride, &offset);

		deviceContext->IASetIndexBuffer(this->vertexBuffer->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		auto colorShader = ShaderManager::GetInstance().GetShader<ColorShader>(SHADER::COLOR);

		colorShader->Draw(deviceContext, this->vertexBuffer->GetNumIndices(), _transform.GetWorldMatrix(), viewMatrix, projectionMatrix);
	}
};