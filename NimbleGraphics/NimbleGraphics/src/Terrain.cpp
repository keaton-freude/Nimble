
#include "Terrain.h"
#include "Logger.h"
#include <SimpleMath.h>
#include "ShaderManager.h"
#include <DirectXColors.h>

using DirectX::BoundingBox;
using namespace DirectX::Colors;

void SetWireframeRender(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
void SetShadedRender(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);

Terrain::Terrain(): _width(0), _height(0), _numChunksX(0), _numChunksZ(0), _vertexCount(0)
{
}

Terrain::Terrain(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, unsigned int numChunksX, 
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

void Terrain::Load(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
{
	//this->CalculateTextureCoordinates();

	LoadChunks2(device);
}

Terrain::~Terrain()
{
	LOG_INFO("Terrain destruct!");
}

void Terrain::Draw(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, shared_ptr<Matrix> viewMatrix, shared_ptr<Matrix> projectionMatrix, shared_ptr<Light> light, shared_ptr<Frustum> frustum)
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

RayHit Terrain::IsRayIntersectingTerrain(Ray r)
{
	auto& vertices = _mem_heightmap->GetHeightmapData();

	for(int i = 0; i < _mem_heightmap->GetHeight() * _mem_heightmap->GetWidth(); ++i)
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

	return RayHit::NoHit;
}

int Terrain::GetVertexCount()
{
	return _vertexCount;
}

bool Terrain::InitializeBuffers()
{
	//int index, i, j, index1, index2, index3, index4;
	//float tu, tv;

	//// Initialize the index to the vertex buffer.
	//index = 0;

	//auto p_vertices = _vertices.get();
	//auto p_heightmap = _heightMap->GetHeightMapData().get();

	//auto heightmap_height = _heightMap->GetHeight();

	//// Load the vertex and index array with the terrain data.
	//for (j = 0; j < _height; j++)
	//{
	//	for (i = 0; i < _width; i++)
	//	{
	//		index1 = (heightmap_height) * j + i;
	//		index2 = (heightmap_height) * j + (i + 1);
	//		index3 = (heightmap_height) * (j + 1) + i;
	//		index4 = (heightmap_height) * (j + 1) + (i + 1);

	//		tv = p_heightmap[index3].texture.y;

	//		// Modify the texture coordinates to cover the top edge.
	//		if (tv == 1.0f) { tv = 0.0f; }
	//		
	//		 // Upper left.
	//		p_vertices[index].position = p_heightmap[index3].position;
	//		p_vertices[index].texture = Vector2(p_heightmap[index3].texture.x, tv);
	//		p_vertices[index].normal = p_heightmap[index3].normal;

	//		index++;

	//		// Upper right.
	//		tu = p_heightmap[index4].texture.x;
	//		tv = p_heightmap[index4].texture.y;

	//		// Modify the texture coordinates to cover the top and right edge.
	//		if (tu == 0.0f) { tu = 1.0f; }
	//		if (tv == 1.0f) { tv = 0.0f; }

	//		p_vertices[index].position = p_heightmap[index4].position;
	//		p_vertices[index].texture = Vector2(tu, tv);
	//		p_vertices[index].normal = p_heightmap[index4].normal;
	//		index++;

	//		// Bottom left.
	//		p_vertices[index].position = p_heightmap[index1].position;
	//		p_vertices[index].texture = Vector2(p_heightmap[index1].texture.x, p_heightmap[index1].texture.y);
	//		p_vertices[index].normal = p_heightmap[index1].normal;
	//		index++;

	//		// Bottom left.
	//		p_vertices[index].position = p_heightmap[index1].position;
	//		p_vertices[index].texture = Vector2(p_heightmap[index1].texture.x, p_heightmap[index1].texture.y);
	//		p_vertices[index].normal = p_heightmap[index1].normal;
	//		index++;

	//		// Upper right.
	//		tu = p_heightmap[index4].texture.x;
	//		tv = p_heightmap[index4].texture.y;

	//		// Modify the texture coordinates to cover the top and right edge.
	//		if (tu == 0.0f) { tu = 1.0f; }
	//		if (tv == 1.0f) { tv = 0.0f; }

	//		p_vertices[index].position = p_heightmap[index4].position;
	//		p_vertices[index].texture = Vector2(tu, tv);
	//		p_vertices[index].normal = p_heightmap[index4].normal;
	//		index++;

	//		// Bottom right.
	//		tu = p_heightmap[index2].texture.x;

	//		// Modify the texture coordinates to cover the right edge.
	//		if (tu == 0.0f) { tu = 1.0f; }

	//		p_vertices[index].position = p_heightmap[index2].position;
	//		p_vertices[index].texture = Vector2(tu, p_heightmap[index2].texture.y);
	//		p_vertices[index].normal = p_heightmap[index2].normal;
	//		index++;
	//	}
	//}

	return true;
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

void Terrain::CalculateTextureCoordinates()
{
	int incrementCount, tuCount, tvCount;
	float incrementValue, tuCoordinate, tvCoordinate;

	auto heightmap_height = _heightMap->GetHeight();
	auto heightmap_width = _heightMap->GetWidth();

	// Calculate how much to increment the texture coordinates by.
	incrementValue = (float)TEXTURE_REPEAT / (float)_width;
	

	// Calculate how many times to repeat the texture.
	incrementCount = heightmap_width / TEXTURE_REPEAT;

	// Initialize the tu and tv coordinate values.
	tuCoordinate = 0.0f;
	tvCoordinate = 1.0f;

	// Initialize the tu and tv coordinate indexes.
	tuCount = 0;
	tvCount = 0;

	//// Loop through the entire height map and calculate the tu and tv texture coordinates for each vertex.
	//for (auto chunk : _chunks)
	//{
	//	for(unsigned int j = 0; j < _chunkHeight; ++j)
	//	{
	//		for(unsigned int i = 0; i < _chunkWidth; ++i)
	//		{
	//			// we go left to right bottom to top
	//			//auto index = _chunkHeight
	//		}
	//	}
	//}

	for (unsigned int j = 0; j < heightmap_height; j++)
	{
		for (unsigned int i = 0; i < heightmap_width; i++)
		{
			// Store the texture coordinate in the height map.
			_heightMap->GetHeightMapData().get()[(heightmap_height * j) + i].texture.x = tuCoordinate;
			_heightMap->GetHeightMapData().get()[(heightmap_height * j) + i].texture.y = tvCoordinate;

			// Increment the tu texture coordinate by the increment value and increment the index by one.
			tuCoordinate += incrementValue;
			tuCount++;

			// Check if at the far right end of the texture and if so then start at the beginning again.
			if (tuCount == incrementCount)
			{
				tuCoordinate = 0.0f;
				tuCount = 0;
			}
		}

		// Increment the tv texture coordinate by the increment value and increment the index by one.
		tvCoordinate -= incrementValue;
		tvCount++;

		// Check if at the top of the texture and if so then start at the bottom again.
		if (tvCount == incrementCount)
		{
			tvCoordinate = 1.0f;
			tvCount = 0;
		}
	}
}

bool Terrain::LoadTextures(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, string grass, string slope, string rock)
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

void Terrain::LoadChunks(ComPtr<ID3D11Device> device)
{
    shared_ptr<unsigned long> indices = shared_ptr<unsigned long>(new unsigned long[_width * 6 * _height]);
    shared_ptr<TerrainVertex> vertices = shared_ptr<TerrainVertex>(new TerrainVertex[_width * 6 * _height ]);

	unsigned long chunk_vertex_index = 0;

	auto p_vertices = vertices.get();
	auto p_indices = indices.get();
	auto p_heightmap = _heightMap->GetHeightMapData().get();
	auto heightmap_height = _heightMap->GetHeight();

	for (int i = 0; i < _numChunksX; ++i)
	{
		for (int j = 0; j < _numChunksZ; ++j)
		{
			// for each chunk populate the arrays with necessary data

            for (int k = (j * _chunkHeight); k < (j * _chunkHeight) + _chunkHeight; ++k)
			{
                for (int l = (i * _chunkWidth); l < (i * _chunkWidth) + _chunkWidth; ++l)
				{
					auto bottomLeft = (heightmap_height) * k + l;
					auto bottomRight = (heightmap_height) * k + (l + 1);
					auto upperLeft = (heightmap_height) * (k + 1) + l;
					auto upperRight = (heightmap_height) * (k + 1) + (l + 1);


					// ORDER IS: Upper Left, Upper Right, Bottom Left
					// ........  Bottom Left, Upper Right, Bottom Right
					// Upper Left
					auto tv = p_heightmap[upperLeft].texture.y;
					//if (tv == 1.0f) { tv = 0.0f; }

					p_vertices[chunk_vertex_index] = p_heightmap[upperLeft];
					p_vertices[chunk_vertex_index].texture.y = tv;


					p_indices[chunk_vertex_index] = chunk_vertex_index;
					chunk_vertex_index++;

					// Upper Right
					auto tu = p_heightmap[upperRight].texture.x;
					tv = p_heightmap[upperRight].texture.y;

					//if (tu == 0.0f) 
					//{ tu = 1.0f; }
					//if (tv == 1.0f) 
					//{ tv = 0.0f; }

					p_vertices[chunk_vertex_index] = p_heightmap[upperRight];
					p_vertices[chunk_vertex_index].texture = Vector2(tu, tv);
					p_indices[chunk_vertex_index] = chunk_vertex_index;
					chunk_vertex_index++;

					// Bottom Left
					p_vertices[chunk_vertex_index] = p_heightmap[bottomLeft];
					p_indices[chunk_vertex_index] = chunk_vertex_index;
					chunk_vertex_index++;

					// Bottom Left
					p_vertices[chunk_vertex_index] = p_heightmap[bottomLeft];
					p_indices[chunk_vertex_index] = chunk_vertex_index;
					chunk_vertex_index++;

					// Upper Right
					tu = p_heightmap[upperRight].texture.x;
					tv = p_heightmap[upperRight].texture.y;
					// Modify the texture coordinates to cover the top and right edge.
					//if (tu == 0.0f) { tu = 1.0f; }
					//if (tv == 1.0f) { tv = 0.0f; }

					p_vertices[chunk_vertex_index] = p_heightmap[upperRight];
					p_vertices[chunk_vertex_index].texture = Vector2(tu, tv);
					p_indices[chunk_vertex_index] = chunk_vertex_index;
					chunk_vertex_index++;

					// Bottom Right
					tu = p_heightmap[bottomRight].texture.x;

					// Modify the texture coordinates to cover the right edge.
					//if (tu == 0.0f) { tu = 1.0f; }

					p_vertices[chunk_vertex_index] = p_heightmap[bottomRight];
					p_vertices[chunk_vertex_index].texture.x = tu;
					p_indices[chunk_vertex_index] = chunk_vertex_index;
					chunk_vertex_index++;
				}
			}

			auto chunk = TerrainChunk(i * _chunkWidth, j * _chunkHeight, _chunkWidth, _chunkHeight, device, vertices, _chunkWidth * 6 * _chunkHeight, indices);
			//chunk.transform().Translate(i * 2, 0, j * 2);
			_chunks.push_back(chunk);
			chunk_vertex_index = 0;
		}
	}
}

void Terrain::LoadChunks2(ComPtr<ID3D11Device> device)
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
	for(int i = 0; i < _width * 6 * _height; ++i)
	{
		p_indices[i] = i;
	}

	for(int z = 0; z < _numChunksZ; ++z)
	{
		for(int x = 0; x < _numChunksX; ++x)
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
			_chunks.push_back(chunk);
			chunk_vertex_index = 0;
		}
	}
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

void Terrain::HeightmapAdd(Vector3 location, float radius, ComPtr<ID3D11DeviceContext> deviceContext, ComPtr<ID3D11Device> device)
{

}

void Terrain::SmoothHeightmapAdd(Vector3 location, float radius, float intensity, ComPtr<ID3D11DeviceContext> deviceContext, ComPtr<ID3D11Device> device)
{
	StartDebugTimer();
	_mem_heightmap->SmoothAdd(location, radius, intensity);
	_mem_heightmap->CalculateTextureCoordinates();
	EndDebugTimer("SmoothAdd: ");

	StartDebugTimer();
	_mem_heightmap->Save();
	EndDebugTimer("Heightmap save: ");
	
	StartDebugTimer();
	_chunks.clear();
	LoadChunks2(device);
	EndDebugTimer("Chunks Recreated: ");
}
