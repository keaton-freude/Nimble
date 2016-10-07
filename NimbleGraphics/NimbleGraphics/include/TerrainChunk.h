#pragma once
#include "Typedefs.h"
#include "Structs.h"
#include "Mesh.h"
#include "TerrainMesh.h"
#include "TerrainVertexField.h"
#include "MemoryHeightmap.h"

class TerrainVertexField;
using std::shared_ptr;
using DirectX::BoundingBox;

class TerrainChunk
{
public:
	TerrainChunk() = default;
	TerrainChunk(int chunk_x, int chunk_z, unsigned int width, unsigned int height, ComPtr<ID3D11Device> device,
	             vector<TerrainVertex>& vertices, vector<unsigned long>& indices);
	TerrainChunk(const TerrainChunk& other);
	~TerrainChunk();

	void Draw(ComPtr<ID3D11DeviceContext> deviceContext);
	vector<TerrainCell>& GetTerrainCellData();
	void Update(ComPtr<ID3D11DeviceContext> deviceContext, MemoryHeightmap& mem_heightmap, unsigned int chunk_x, unsigned int chunk_z);
	BoundingBox GetBoundingBox() const;

private:
	int _vertexCount;
	int _indexCount;
	int _positionX;
	int _positionZ;
	unsigned int _width;
	unsigned int _height;
	BoundingBox _boundingBox;
	TerrainMesh _drawable;
	vector<TerrainCell> _terrainCellData;
	int chunk_x;
	int chunk_z;
};