#include "Terrain.h"
#include <algorithm>

Terrain::Terrain(): _width(0), _height(0), _numChunksX(0), _numChunksZ(0), _vertexCount(0)
{
}

Terrain::Terrain(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, unsigned numChunksX, unsigned numChunksZ, vector<wstring>& texture_paths)
	: _numChunksX(numChunksX), _numChunksZ(numChunksZ), _chunks()
{
	_chunks.reserve(numChunksX * numChunksZ);
	_width = _numChunksX * _chunkWidth;
	_height = _numChunksZ * _chunkHeight;

	this->_mem_heightmap = make_shared<MemoryHeightmap>(_width, _height, 1.0f);

	// Calculate the number of vertices in the terrain mesh.
	_vertexCount = _width * _height * 4;

	splatMap = make_shared<SplatMap>();
	splatMap->LoadFromFile(device, deviceContext, texture_paths);
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
	terrain_shader->SetShaderParameters(deviceContext, light, *splatMap);

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
	StartDebugTimer();
	_mem_heightmap->SmoothAdd(location, radius, intensity);
	//_mem_heightmap->CalculateTextureCoordinates();
	EndDebugTimer("SmoothAdd: ");

	StartDebugTimer();
	_mem_heightmap->CalculateNormalsDifferently(location, radius + 2.0f);
	EndDebugTimer("Calculate normals: ");

	StartDebugTimer();
	UpdateChunks(device, deviceContext);
	EndDebugTimer("Chunks Updated: ");
}

void Terrain::SplatTexture(D3DDevice device, D3DDeviceContext deviceContext, Vector3 location, float radius, int intensity, int texture_number)
{
	// To splat a texture, we just access the splat map and draw the correct color.
	const auto splat = splatMap->GetSplat(texture_number);

	auto color = splatMap->GetColorComponent(texture_number);

	D3D11_MAPPED_SUBRESOURCE subresource_data;

	ID3D11Texture2D* texture = static_cast<ID3D11Texture2D*>(splat);

	D3D11_TEXTURE2D_DESC desc;
	texture->GetDesc(&desc);

	auto width = desc.Width;
	auto height = desc.Height;

	auto terrain_height = _numChunksZ * 0.5f * _chunkHeight;
	auto terrain_width = _numChunksX * 0.5f * _chunkWidth;

	Vector2 vector_coords = Vector2(location.x, terrain_height - location.z);
	Vector2 coords_pixel_space = Vector2((location.x / terrain_width) * width,
		((terrain_height - location.z) / terrain_height) * height);
	Vector2 extents = Vector2(radius, radius);

	Vector2 start_position = vector_coords - Vector2(radius, radius);
	Vector2 end_position = vector_coords + Vector2(radius, radius);

	

	// We need the value between 0 and 1024 for both x and y
	// but we have a physical location


	int start_j = (start_position.y / terrain_height) * height;
	int start_i = (start_position.x / terrain_width) * width;

	if (start_j < 0)
		start_j = 0;
	if (start_i < 0)
		start_i = 0;

	int end_j = (end_position.y / terrain_height) * height;
	int end_i = (end_position.x / terrain_width) * height;

	D3D11_TEXTURE2D_DESC copyDesc = desc;
	D3DTexture2D copyTexture;
	D3D11_MAPPED_SUBRESOURCE copyResourceData;

	// Only thing we'll do different is the usage and access
	copyDesc.Usage = D3D11_USAGE_STAGING;
	copyDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	copyDesc.BindFlags = 0;

	device->CreateTexture2D(&copyDesc, NULL, copyTexture.GetAddressOf());

	deviceContext->CopyResource(copyTexture.Get(), texture);

	deviceContext->Map(copyTexture.Get(), 0, D3D11_MAP_READ_WRITE, 0, &copyResourceData);

	Vector2 current_loc;

	unsigned char* data_ptr = (unsigned char*)(copyResourceData.pData);

	for (int j = start_j; j < end_j; ++j)
	{
		for (int i = start_i; i < end_i; ++i)
		{
			if (i > width)
				continue;
			if (j > height)
				continue;

			current_loc = Vector2(((float)i / (float)width) * terrain_width, ((float)j / (float)height) * terrain_height);
			auto distance = Vector2::Distance(current_loc, vector_coords);
			if (Vector2::Distance(current_loc, vector_coords) > radius)
				continue;

			auto index = (j * height + i) * 4;

			unsigned char new_value = 0;

			if (*(data_ptr + index + color) + intensity > 255)
				new_value = 255;
			else
				new_value = *(data_ptr + index + color) + intensity;
			
			*(data_ptr + index + color) = new_value;
		}
	}

	deviceContext->Map(splat, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource_data);

	unsigned char* real_data_ptr = (unsigned char*)(subresource_data.pData);

	memcpy(real_data_ptr, data_ptr, 4 * width * height);

	deviceContext->Unmap(splat, 0);

	deviceContext->Unmap(copyTexture.Get(), 0);
	
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

		triangle1[0] = current_cell.data.upperLeft.position;
		triangle1[1] = current_cell.data.upperRight.position;
		triangle1[2] = current_cell.data.bottomLeft.position;

		triangle2[0] = current_cell.data.bottomLeft.position;
		triangle2[1] = current_cell.data.upperRight.position;
		triangle2[2] = current_cell.data.bottomRight.position;

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
