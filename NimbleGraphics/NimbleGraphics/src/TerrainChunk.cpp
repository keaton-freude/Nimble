#include "TerrainChunk.h"
#include "Logger.h"

TerrainChunk::TerrainChunk()
	: _vertexCount(0), _positionX(0), _positionZ(0), _width(0), _height(0)
{
}

TerrainChunk::TerrainChunk(int posX, int posZ, unsigned int width, unsigned int height, ComPtr<ID3D11Device> device, 
	shared_ptr<TerrainVertex> vertices, int vertexCount, shared_ptr<unsigned long> indices)
	: _vertexCount(vertexCount), _positionX(posX), _positionZ(posZ), _width(width), _height(height), 
	_drawable(device, vertices, vertexCount, indices)
{
	BoundingBox::CreateFromPoints(_boundingBox, Vector3(0.0f, 0.0f, 0.0f), Vector3(_width, 1.0f, height));
}

TerrainChunk::~TerrainChunk()
{
	LOG_INFO("Terrain Chunk Destruct!");
}

void TerrainChunk::WriteToBuffer(ComPtr<ID3D11DeviceContext> deviceContext, shared_ptr<TerrainVertex> vertices, shared_ptr<VertexBuffer> vertexBuffer)
{
	D3D11_MAPPED_SUBRESOURCE resource;
	deviceContext->Map(vertexBuffer->GetVertexBuffer().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	TerrainVertex* dataPtr = (TerrainVertex*)resource.pData;
	auto terrainDataPtr = vertices.get();

	unsigned int index = 0;

	for (int j = 0; j < _height; ++j)
	{
		for (int i = 0; i < _width; ++i)
		{
			dataPtr[j + _positionZ, i + _positionX] = terrainDataPtr[j, i];
		}
	}

	deviceContext->Unmap(vertexBuffer->GetVertexBuffer().Get(), 0);
}

void TerrainChunk::Draw(ComPtr<ID3D11DeviceContext> deviceContext)
{
	_drawable.Draw(deviceContext, _vertexCount);
}