#include "ColorDrawable.h"
#include "ShaderManager.h"
#include "Logger.h"

ColorDrawable::ColorDrawable()
{
	vertexBuffer = 0;
	_transform = struct Transform();


}

ColorDrawable::~ColorDrawable()
{
	LOG_INFO("Color Drawable destruct!");
}

/* Set buffers and draw. This is an INDEXED draw, the given VertexBuffer must have correct Index Data */
void ColorDrawable::Draw(ComPtr<ID3D11DeviceContext> deviceContext, shared_ptr<Matrix> viewMatrix, 
	shared_ptr<Matrix> projectionMatrix)
{
	// bind our vertex buffer
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(TerrainVertex);
	offset = 0;
	auto vb = this->vertexBuffer->GetVertexBuffer().Get();
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);

	deviceContext->IASetIndexBuffer(this->vertexBuffer->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	auto colorShader = ShaderManager::GetInstance().GetShader<ColorShader>(SHADER::COLOR);

	colorShader->Draw(deviceContext, this->vertexBuffer->GetNumIndices(), _transform.GetWorldMatrix(), viewMatrix, projectionMatrix);
}
