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
	TerrainChunk();
	TerrainChunk(int posX, int posZ, unsigned int width, unsigned int height, ComPtr<ID3D11Device> device,
		shared_ptr<TerrainVertex> vertices, int vertexCount, shared_ptr<unsigned long> indices);
	~TerrainChunk();

	void Draw(ComPtr<ID3D11DeviceContext> deviceContext);
	void WriteToBuffer(ComPtr<ID3D11DeviceContext> deviceContext, shared_ptr<TerrainVertex> vertices, shared_ptr<VertexBuffer> vertexBuffer);
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