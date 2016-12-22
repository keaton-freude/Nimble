#include "TerrainChunk.h"

TerrainChunk::TerrainChunk(int chunk_x, int chunk_z, unsigned width, unsigned height, ComPtr<ID3D11Device> device, vector<TerrainVertex>& vertices, vector<unsigned long>& indices)
	: _vertexCount(vertices.size()), _indexCount(indices.size()), _positionX(0), _positionZ(0), _width(width), _height(height),
	_drawable(device, vertices, indices), _terrainCellData(), chunk_x(chunk_x), chunk_z(chunk_z)
{
	BoundingBox::CreateFromPoints(_boundingBox, Vector3(0.0f, 0.0f, 0.0f), Vector3(_width, 1.0f, height));
	auto vert_index = 0;
}

TerrainChunk::TerrainChunk(const TerrainChunk& other)
{
}

TerrainChunk::~TerrainChunk()
{
	LOG_INFO("Terrain Chunk Destruct!");
}

void TerrainChunk::Draw(ComPtr<ID3D11DeviceContext> deviceContext)
{
	_drawable.Draw(deviceContext, _indexCount);
}

vector<TerrainCell>& TerrainChunk::GetTerrainCellData()
{
	return _terrainCellData;
}

void TerrainChunk::Update(ComPtr<ID3D11DeviceContext> deviceContext, MemoryHeightmap& mem_heightmap, unsigned chunk_x, unsigned chunk_z)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	TerrainVertex* dataPtr;

	auto p_map_vertices = mem_heightmap.GetVertexField()->GetVertices();

	deviceContext->Map(_drawable.GetVertexBuffer()->GetVertexBuffer().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	dataPtr = static_cast<TerrainVertex*>(mappedResource.pData);

	for (auto j = 0; j < (_height + 1); ++j)
	{
		for (auto i = 0; i < (_width + 1); ++i)
		{
			auto vert_index = j * (_height + 1) + i;
			auto map_index = mem_heightmap.GetIndex(chunk_z, chunk_x, _width, _height, j, i);

			dataPtr[vert_index] = p_map_vertices[map_index];
		}
	}

	deviceContext->Unmap(_drawable.GetVertexBuffer()->GetVertexBuffer().Get(), 0);
}

BoundingBox TerrainChunk::GetBoundingBox() const
{
	return _boundingBox;
}
