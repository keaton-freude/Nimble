#include "Terrain.h"

Terrain::Terrain(): _width(0), _height(0), _numChunksX(0), _numChunksZ(0), _vertexCount(0)
{
}

Terrain::Terrain(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, unsigned numChunksX, unsigned numChunksZ, vector<wstring>& texture_paths)
	: _numChunksX(numChunksX), _numChunksZ(numChunksZ), _chunks()
{
	_chunks.reserve(numChunksX * numChunksZ);
	_width = _numChunksX * _chunkWidth;
	_height = _numChunksZ * _chunkHeight;

	this->_mem_heightmap = make_shared<MemoryHeightmap>(_width, _height, .5f);

	// Calculate the number of vertices in the terrain mesh.
	_vertexCount = _width * _height * 4;

	textures = make_shared<TextureArray>(device.Get(), deviceContext.Get(), texture_paths);
	this->Load(device, deviceContext);
}

Terrain::~Terrain()
{
	LOG_INFO("Terrain destruct!");
}

void Terrain::Load(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
{
	//this->CalculateTextureCoordinates();

	LoadChunks(device);
}

void Terrain::Draw(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, const Matrix& viewMatrix, const Matrix& projectionMatrix, const Light& light, const Frustum& frustum)
{
	auto terrain_shader = ShaderManager::GetInstance().GetShader<TerrainShader>(SHADER::TERRAIN);

	terrain_shader->SetViewMatrix(viewMatrix);
	terrain_shader->SetProjectionMatrix(projectionMatrix);
	terrain_shader->SetShaderParameters(deviceContext, light, *textures);

	deviceContext->OMSetBlendState(StatesHelper::GetInstance().GetStates()->Opaque(), Black, 0xffffffff);

	deviceContext->OMSetDepthStencilState(StatesHelper::GetInstance().GetStates()->DepthDefault(), 0);

	//	_chunks.front().Draw(deviceContext);

	for (auto it = _chunks.begin(); it != _chunks.end(); ++it)
	{
		(*it).Draw(deviceContext);
	}
}

void Terrain::SmoothHeightmapAdd(Vector3 location, float radius, float intensity, ComPtr<ID3D11DeviceContext> deviceContext, ComPtr<ID3D11Device> device)
{
	//StartDebugTimer();
	_mem_heightmap->SmoothAdd(location, radius, intensity);
	//_mem_heightmap->CalculateTextureCoordinates();
	//EndDebugTimer("SmoothAdd: ");

	//StartDebugTimer();
	_mem_heightmap->CalculateNormalsDifferently(location, radius + 2.0f);
	//EndDebugTimer("Calculate normals: ");

	//StartDebugTimer();
	UpdateChunks(device, deviceContext);
	//EndDebugTimer("Chunks Updated: ");
}

RayHit Terrain::IsRayIntersectingTerrain(Ray r) const
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

void Terrain::SetWireframeRender(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) const
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

void Terrain::SetShadedRender(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) const
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

unsigned Terrain::GetTerrainChunkSubIndex(int i, int j, int chunk_height)
{
	return j * (chunk_height + 1) + i;
}

void Terrain::LoadChunks(ComPtr<ID3D11Device> device)
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
	for (auto j = 0; j < chunk_height; ++j)
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

void Terrain::UpdateChunks(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
{
	for (auto z = 0; z < _numChunksZ; ++z)
	{
		for (auto x = 0; x < _numChunksX; ++x)
		{
			auto& chunk = _chunks[z * _numChunksZ + x];
			chunk.Update(deviceContext, *_mem_heightmap, x, z);
		}
	}
}
