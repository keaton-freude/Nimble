#include "TerrainDrawable.h"
#include "Graphics.h"
#include "ShaderManager.h"

TerrainDrawable::TerrainDrawable()
{

}

TerrainDrawable::TerrainDrawable(ComPtr<ID3D11Device> device, shared_ptr<TerrainVertex> vertices, int vertexCount, shared_ptr<unsigned long> indices)
{
	vertexBuffer = VertexBuffer::CreateTerrainNode(device, vertices, vertexCount, indices);
}

TerrainDrawable::~TerrainDrawable()
{
	LOG_INFO("TerrainDrawable destruct!");
}

void TerrainDrawable::Draw(ComPtr<ID3D11DeviceContext> deviceContext, int vertexCount)
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
	// Get pointer to colorShader

	auto terrainShader = ShaderManager::GetInstance().GetShader<TerrainShader>(SHADER::TERRAIN);

	terrainShader->RenderShader(deviceContext, vertexCount);
}

void TerrainDrawable::Update(ComPtr<ID3D11DeviceContext> deviceContext, int x, int y, int width, shared_ptr<TerrainVertex> vertices)
{
	D3D11_MAPPED_SUBRESOURCE resource;

	// take our VertexBuffer object, Map it, write our data over the old data, Unmap it
	deviceContext->Map(this->vertexBuffer->GetVertexBuffer().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	TerrainVertex* dataPtr = (TerrainVertex*)resource.pData;

	for (int iy = y; iy < width; ++iy)
	{
		for (int ix = x; ix < width; ++ix)
		{
			dataPtr[iy * width + ix] = vertices.get()[iy * width + ix];
		}
	}
	
	deviceContext->Unmap(this->vertexBuffer->GetVertexBuffer().Get(), 0);
}