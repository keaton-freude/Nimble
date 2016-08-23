#pragma once

#include "TerrainQuadTree.h"
#include "ColorDrawable.h"
#include "TerrainChunk.h"
#include "TerrainShader.h"
#include "Heightmap.h"
#include "RayHit.h"
#include <SimpleMath.h>
#include "Light.h"
#include <vector>
#include "Texture.h"
#include "Structs.h"
#include "MemoryHeightmap.h"

#include <memory>
#include <wrl\client.h>

using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector2;
using std::string;
using std::shared_ptr;
using std::vector;
using Microsoft::WRL::ComPtr;


class Terrain
{
public:
	Terrain();
	Terrain(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, unsigned int numChunksX,
		unsigned int numChunksZ, string grassTexture, string slopeTexture, string rockTexture);
	~Terrain();

	void Draw(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext,
		shared_ptr<Matrix> viewMatrix, shared_ptr<Matrix> projectionMatrix, shared_ptr<Light> light, shared_ptr<Frustum> frustum);



	// external functions (editor, etc)
	void HeightmapAdd(Vector3 location, float radius, ComPtr<ID3D11DeviceContext> deviceContext, ComPtr<ID3D11Device> device);
	void SmoothHeightmapAdd(Vector3 location, float radius, float intensity, ComPtr<ID3D11DeviceContext> deviceContext,
		ComPtr<ID3D11Device> device);
	RayHit IsRayIntersectingTerrain(Ray r);
	int GetVertexCount();

private:
	void CalculateTextureCoordinates();
	bool InitializeBuffers();
	bool LoadTextures(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, string grass, string slope, string rock);
	void LoadChunks(ComPtr<ID3D11Device> device);
	void LoadChunks2(ComPtr<ID3D11Device> device);

private:
	shared_ptr<Heightmap> _heightMap;
	shared_ptr<MemoryHeightmap> _mem_heightmap;
	void Load(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);

	unsigned int _width;
	unsigned int _height;
	unsigned int _numChunksX;
	unsigned int _numChunksZ;
	const unsigned int _chunkWidth = 16;
	const unsigned int _chunkHeight = 16;

	shared_ptr<Texture> _grassTexture;
	shared_ptr<Texture> _slopeTexture;
	shared_ptr<Texture> _rockTexture;
	string texture_name;

	int _vertexCount;
	//shared_ptr<unsigned long> _indices;
	//shared_ptr<TerrainVertex> _vertices;
	vector<TerrainChunk> _chunks;
};