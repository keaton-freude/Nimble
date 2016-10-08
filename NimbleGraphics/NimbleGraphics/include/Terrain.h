#pragma once
#include "Typedefs.h"
#include "TerrainChunk.h"
#include "RayHit.h"
#include "Light.h"
#include <vector>
#include "Texture.h"
#include "MemoryHeightmap.h"
#include "Frustum.h"

using std::string;
using std::vector;
using DirectX::BoundingBox;
using namespace DirectX::Colors;


class Terrain
{
public:
	Terrain();
	Terrain(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, unsigned int numChunksX,
	        unsigned int numChunksZ, vector<wstring>& texture_paths);
	~Terrain();

	void Load(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);

	void Draw(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext,
	          const Matrix& viewMatrix, const Matrix& projectionMatrix, const Light& light, const Frustum& frustum);
	void SmoothHeightmapAdd(Vector3 location, float radius, float intensity, ComPtr<ID3D11DeviceContext> deviceContext,
	                        ComPtr<ID3D11Device> device);

	RayHit IsRayIntersectingTerrain(Ray r) const;

	int GetVertexCount() const
	{
		return _vertexCount;
	}

	void SetWireframeRender(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) const;
	void SetShadedRender(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) const;

private:
	static unsigned int GetTerrainChunkSubIndex(int i, int j, int chunk_height);
	void LoadChunks(ComPtr<ID3D11Device> device);
	void UpdateChunks(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);

private:
	shared_ptr<MemoryHeightmap> _mem_heightmap;

	unsigned int _width;
	unsigned int _height;
	unsigned int _numChunksX;
	unsigned int _numChunksZ;
	const unsigned int _chunkWidth = 16;
	const unsigned int _chunkHeight = 16;

	shared_ptr<TextureArray> textures;

	int _vertexCount;
	vector<TerrainChunk> _chunks;
};