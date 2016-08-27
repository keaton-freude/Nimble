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
		_vertexCount = _width * _height * 6;

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
		shared_ptr<Matrix> viewMatrix, shared_ptr<Matrix> projectionMatrix, shared_ptr<Light> light, shared_ptr<Frustum> frustum)
	{
		auto terrain_shader = ShaderManager::GetInstance().GetShader<TerrainShader>(SHADER::TERRAIN);

		terrain_shader->SetViewMatrix(*viewMatrix);
		terrain_shader->SetProjectionMatrix(*projectionMatrix);
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
		StartDebugTimer();
		_mem_heightmap->SmoothAdd(location, radius, intensity);
		for(auto& chunk : _chunks)
		{
			//chunk.GetTerrainCellData()
		}
		_mem_heightmap->CalculateTextureCoordinates();
		EndDebugTimer("SmoothAdd: ");

		StartDebugTimer();
		UpdateChunks(device, deviceContext);
		EndDebugTimer("Chunks Updated: ");
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

			triangle1[0] = current_cell.upperLeft.position;
			triangle1[1] = current_cell.upperRight.position;
			triangle1[2] = current_cell.bottomLeft.position;

			triangle2[0] = current_cell.bottomLeft.position;
			triangle2[1] = current_cell.upperRight.position;
			triangle2[2] = current_cell.bottomRight.position;

			float distance;

			if (rayTriangleIntersect(r, triangle1, distance))
			{
				LOG_INFO("Ray Triangle Intersect!");

				Vector3 hit_location = (r.position + (r.direction * distance));
				return RayHit(r.position, hit_location, distance, true);
			}

			if (rayTriangleIntersect(r, triangle2, distance))
			{
				LOG_INFO("Ray Triangle Intersect!");

				Vector3 hit_location = (r.position + (r.direction * distance));
				return RayHit(r.position, hit_location, distance, true);
			}
		}

		return RayHit::NoHit();
	}

	int GetVertexCount()
	{
		return _vertexCount;
	}

	void SetWireframeRender(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
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

	void SetShadedRender(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
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

	void LoadChunks(ComPtr<ID3D11Device> device)
	{
		auto& p_heightmap = _mem_heightmap.get()->GetHeightmapData();
		shared_ptr<unsigned long> indices = shared_ptr<unsigned long>(new unsigned long[_width * 6 * _height]);
		shared_ptr<TerrainVertex> vertices = shared_ptr<TerrainVertex>(new TerrainVertex[_width * 6 * _height]);
		auto p_vertices = vertices.get();
		auto p_indices = indices.get();
		unsigned long chunk_vertex_index = 0;
		unsigned int chunk_width = _chunkWidth;
		unsigned int chunk_height = _chunkHeight;
		unsigned int chunk_index = 0;

		// indices are in-order
		for (int i = 0; i < _width * 6 * _height; ++i)
		{
			p_indices[i] = i;
		}

		for (int z = 0; z < _numChunksZ; ++z)
		{
			for (int x = 0; x < _numChunksX; ++x)
			{
				// Need to create a chunk from our heightmap data
				for (int j = 0; j < chunk_height; ++j)
				{
					for (int i = 0; i < chunk_width; ++i)
					{
						auto base_index = chunk_width * x + z * _height * chunk_height;

						base_index += j * _height + i;

						auto index = base_index;
						// for each cell, push verts in the right order
						auto& cell = p_heightmap[index];

						// UPPER LEFT
						p_vertices[chunk_vertex_index] = cell.upperLeft;
						chunk_vertex_index++;

						// UPPER RIGHT
						p_vertices[chunk_vertex_index] = cell.upperRight;
						chunk_vertex_index++;

						// BOTTOM LEFT
						p_vertices[chunk_vertex_index] = cell.bottomLeft;
						chunk_vertex_index++;

						// BOTTOM LEFT
						p_vertices[chunk_vertex_index] = cell.bottomLeft;
						chunk_vertex_index++;

						// UPPER RIGHT
						p_vertices[chunk_vertex_index] = cell.upperRight;
						chunk_vertex_index++;

						// BOTTOM RIGHT
						p_vertices[chunk_vertex_index] = cell.bottomRight;
						chunk_vertex_index++;
					}
				}

				auto chunk = TerrainChunk(x * chunk_width, z * chunk_height, chunk_width, chunk_height, device,
					vertices, _width * _height * 6, indices);
				chunk.GetTerrainCellData();
				_chunks.push_back(chunk);
				chunk_vertex_index = 0;
			}
		}
	}

	void UpdateChunks(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
	{
		for(auto & chunk : _chunks)
		{
			chunk.Update(deviceContext);
		}
	}

private:
	shared_ptr<MemoryHeightmap> _mem_heightmap;

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