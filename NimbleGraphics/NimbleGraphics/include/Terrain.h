#pragma once
#include "ColorDrawable.h"
#include "TerrainChunk.h"
#include "TerrainShader.h"
#include "RayHit.h"
#include <SimpleMath.h>
#include "Light.h"
#include <vector>
#include "Texture.h"
#include "Structs.h"
#include "MemoryHeightmap.h"

#include <memory>
#include <wrl\client.h>
#include "Frustum.h"
#include "TerrainVertexField.h"

using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector2;
using std::string;
using std::shared_ptr;
using std::vector;
using Microsoft::WRL::ComPtr;
using DirectX::BoundingBox;
using namespace DirectX::Colors;



class Terrain
{
public:
	Terrain(): _width(0), _height(0), _numChunksX(0), _numChunksZ(0), _vertexCount(0)
	{
		
	}

	Terrain(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, unsigned int numChunksX,
		unsigned int numChunksZ, string grassTexture, string slopeTexture, string rockTexture)
		: _numChunksX(numChunksX), _numChunksZ(numChunksZ), _grassTexture(nullptr),
		  _slopeTexture(nullptr), _rockTexture(nullptr), _chunks()
	{
		_width = _numChunksX * _chunkWidth;
		_height = _numChunksZ * _chunkHeight;

		this->_mem_heightmap = make_shared<MemoryHeightmap>(_width, _height);
		//this->_heightMap = make_shared<Heightmap>(_width + numChunksX, _height + numChunksZ, 1.0f);

		// Calculate the number of vertices in the terrain mesh.
		_vertexCount = _width * _height * 4;

		this->LoadTextures(device, deviceContext, grassTexture, slopeTexture, rockTexture);

		this->Load(device, deviceContext);
	}

	~Terrain()
	{
		LOG_INFO("Terrain destruct!");
	}

	void Load(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
	{
		//this->CalculateTextureCoordinates();

		LoadChunks(device);
	}

	void Draw(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext,
		const Matrix& viewMatrix, const Matrix& projectionMatrix, const Light& light, const Frustum& frustum)
	{
		auto terrain_shader = ShaderManager::GetInstance().GetShader<TerrainShader>(SHADER::TERRAIN);

		terrain_shader->SetViewMatrix(viewMatrix);
		terrain_shader->SetProjectionMatrix(projectionMatrix);
		terrain_shader->SetShaderParameters(deviceContext, light, _grassTexture->GetTexture(), _slopeTexture->GetTexture(), _rockTexture->GetTexture());

		deviceContext->OMSetBlendState(StatesHelper::GetInstance().GetStates()->Opaque(), Black, 0xffffffff);

		deviceContext->OMSetDepthStencilState(StatesHelper::GetInstance().GetStates()->DepthDefault(), 0);

		//	_chunks.front().Draw(deviceContext);

		for (auto it = _chunks.begin(); it != _chunks.end(); ++it)
		{
			(*it).Draw(deviceContext);
		}
	}

	void SmoothHeightmapAdd(Vector3 location, float radius, float intensity, ComPtr<ID3D11DeviceContext> deviceContext,
		ComPtr<ID3D11Device> device)
	{
		//StartDebugTimer();
		_mem_heightmap->SmoothAdd(location, radius, intensity);
		//_mem_heightmap->CalculateTextureCoordinates();
		//EndDebugTimer("SmoothAdd: ");

		//StartDebugTimer();
		_mem_heightmap->CalculateNormalsDifferently(location, radius);
		//EndDebugTimer("Calculate normals: ");

		//StartDebugTimer();
		UpdateChunks(device, deviceContext);
		//EndDebugTimer("Chunks Updated: ");
	}

	RayHit IsRayIntersectingTerrain(Ray r)
	{
		auto& vertices = _mem_heightmap->GetHeightmapData();

		for (int i = 0; i < _mem_heightmap->GetHeight() * _mem_heightmap->GetWidth(); ++i)
		{
			// every cell has 2 triangles
			auto& current_cell = vertices[i];

			Vector3 triangle1[3];
			Vector3 triangle2[3];

			triangle1[0] = current_cell.data.upperLeft->position;
			triangle1[1] = current_cell.data.upperRight->position;
			triangle1[2] = current_cell.data.bottomLeft->position;

			triangle2[0] = current_cell.data.bottomLeft->position;
			triangle2[1] = current_cell.data.upperRight->position;
			triangle2[2] = current_cell.data.bottomRight->position;

			float distance;

			if (rayTriangleIntersect(r, triangle1, distance))
			{
				//LOG_INFO("Ray Triangle Intersect. Distance: ", distance);

				Vector3 hit_location = (r.position + (r.direction * distance));
				return RayHit(r.position, hit_location, distance, true);
			}

			if (rayTriangleIntersect(r, triangle2, distance))
			{
				//OG_INFO("Ray Triangle Intersect. Distance: ", distance);

				Vector3 hit_location = (r.position + (r.direction * distance));
				return RayHit(r.position, hit_location, distance, true);
			}
		}

		return RayHit::NoHit();
	}

	int GetVertexCount() const
	{
		return _vertexCount;
	}

	void SetWireframeRender(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) const
	{
		HRESULT result;

		ID3D11RasterizerState* rState;
		D3D11_RASTERIZER_DESC rDesc;

		deviceContext->RSGetState(&rState);
		rState->GetDesc(&rDesc);
		rDesc.FillMode = D3D11_FILL_WIREFRAME;

		result = device->CreateRasterizerState(&rDesc, &rState);

		deviceContext->RSSetState(rState);

		rState->Release();
		delete rState;
	}

	void SetShadedRender(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) const
	{
		ID3D11RasterizerState* rState;
		D3D11_RASTERIZER_DESC rDesc;

		deviceContext->RSGetState(&rState);
		rState->GetDesc(&rDesc);
		rDesc.FillMode = D3D11_FILL_SOLID;


		device->CreateRasterizerState(&rDesc, &rState);

		deviceContext->RSSetState(rState);

		rState->Release();
	}

private:
	bool LoadTextures(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, string grass, string slope, string rock)
	{
		bool result;

		// Create the grass texture object.
		_grassTexture = make_shared<Texture>(device, deviceContext, grass);

		// Create the slope texture object.
		_slopeTexture = make_shared<Texture>(device, deviceContext, slope);

		// Create the rock texture object.
		_rockTexture = make_shared<Texture>(device, deviceContext, rock);

		return true;
	}

	unsigned int GetTerrainChunkSubIndex(int i, int j, int chunk_height)
	{
		return j * (chunk_height + 1) + i;
	}

	void LoadChunks(ComPtr<ID3D11Device> device)
	{
		auto& p_heightmap = _mem_heightmap.get()->GetHeightmapData();
		auto vertices = vector<TerrainVertex>((_chunkWidth + 1) * (_chunkHeight + 1));
		auto indices = vector<unsigned long>(_chunkHeight * 6 * _chunkWidth);
		auto p_indices = &indices[0];
		auto p_vertices = &vertices[0];
		unsigned long chunk_vertex_index = 0;
		auto chunk_width = _chunkWidth;
		auto chunk_height = _chunkHeight;
		unsigned int chunk_index = 0;
		auto p_map_vertices = _mem_heightmap->GetVertexField()->GetVertices();

		unsigned int index = 0;

		// build indices
		for(auto j = 0; j < chunk_height; ++j)
		{
			for (auto i = 0; i < chunk_width; ++i)
			{
				// go in groups of 3
				// upper-left
				p_indices[index++] = GetTerrainChunkSubIndex(i, j + 1, chunk_height);
				// upper-right
				p_indices[index++] = GetTerrainChunkSubIndex(i + 1, j + 1, chunk_height);
				// bottom-left
				p_indices[index++] = GetTerrainChunkSubIndex(i, j, chunk_height);

				// bottom-left
				p_indices[index++] = GetTerrainChunkSubIndex(i, j, chunk_height);
				// upper-right
				p_indices[index++] = GetTerrainChunkSubIndex(i + 1, j + 1, chunk_height);
				// bottom-right
				p_indices[index++] = GetTerrainChunkSubIndex(i + 1, j, chunk_height);
				
			}
		}

		for (auto z = 0; z < _numChunksZ; ++z)
		{
			for (auto x = 0; x < _numChunksX; ++x)
			{
				// Need to create a chunk from our heightmap data
				for (auto j = 0; j < (chunk_height + 1); ++j)
				{
					for (auto i = 0; i < (chunk_width + 1); ++i)
					{
						auto vert_index = j * (chunk_height + 1) + i;
						auto map_index = _mem_heightmap->GetIndex(z, x, chunk_width, chunk_height, j, i);

						vertices[vert_index] = p_map_vertices[map_index];
					}
				} 

				_chunks.emplace_back(x, z, chunk_width, chunk_height, device, vertices, indices);
				chunk_vertex_index = 0;
			}
		}
	}

	void UpdateChunks(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
	{
		for (auto z = 0; z < _numChunksZ; ++z)
		{
			for(auto x = 0; x < _numChunksX; ++x)
			{
				auto& chunk = _chunks[z * _numChunksZ + x];
				chunk.NewUpdate(deviceContext, *_mem_heightmap, x, z);
			}
		}
	}

private:
	shared_ptr<MemoryHeightmap> _mem_heightmap;

	unsigned int _width;
	unsigned int _height;
	unsigned int _numChunksX;
	unsigned int _numChunksZ;
	const unsigned int _chunkWidth = 32;
	const unsigned int _chunkHeight = 32;

	shared_ptr<Texture> _grassTexture;
	shared_ptr<Texture> _slopeTexture;
	shared_ptr<Texture> _rockTexture;
	string texture_name;

	int _vertexCount;
	vector<TerrainChunk> _chunks;
};