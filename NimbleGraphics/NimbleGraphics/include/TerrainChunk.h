#pragma once
#include <memory>
#include "Transform.h"
#include "Structs.h"
#include "VertexBuffer.h"
#include "TerrainDrawable.h"

using std::shared_ptr;
using DirectX::BoundingBox;

class TerrainChunk
{
public:
	TerrainChunk()
	{
		
	}

	TerrainChunk(int posX, int posZ, unsigned int width, unsigned int height, ComPtr<ID3D11Device> device,
		shared_ptr<TerrainVertex> vertices, int vertexCount, shared_ptr<unsigned long> indices)
		: _vertexCount(vertexCount), _positionX(posX), _positionZ(posZ), _width(width), _height(height),
		_drawable(device, vertices, vertexCount, indices)
	{
		BoundingBox::CreateFromPoints(_boundingBox, Vector3(0.0f, 0.0f, 0.0f), Vector3(_width, 1.0f, height));
	}

	~TerrainChunk()
	{
		LOG_INFO("Terrain Chunk Destruct!");
	}

	void Draw(ComPtr<ID3D11DeviceContext> deviceContext)
	{
		_drawable.Draw(deviceContext, _vertexCount);
	}

	void WriteToBuffer(ComPtr<ID3D11DeviceContext> deviceContext, shared_ptr<TerrainVertex> vertices, shared_ptr<VertexBuffer> vertexBuffer)
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

	BoundingBox GetBoundingBox() const { return _boundingBox; }
	Transform& transform() { return _drawable._transform; }
private:
	int _vertexCount;
	int _positionX;
	int _positionZ;
	unsigned int _width;
	unsigned int _height;
	BoundingBox _boundingBox;
	TerrainDrawable _drawable;
};