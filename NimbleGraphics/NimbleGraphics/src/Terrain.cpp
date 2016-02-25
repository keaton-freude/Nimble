
#include "Terrain.h"
#include "Logger.h"
#include <SimpleMath.h>
#include "ShaderManager.h"
#include <DirectXColors.h>

using DirectX::BoundingBox;
using namespace DirectX::Colors;

void SetWireframeRender(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
void SetShadedRender(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);

Terrain::Terrain()
{
}

Terrain::Terrain(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, unsigned int numChunksX, 
	unsigned int numChunksZ, string grassTexture, string slopeTexture, string rockTexture)
	: _numChunksX(numChunksX), _numChunksZ(numChunksZ), _grassTexture(0),
	_slopeTexture(0), _rockTexture(0), _chunks()
{
	this->_heightMap = make_shared<Heightmap>((64 * numChunksX) + 1, (64 * numChunksZ) + 1);

	_width = _heightMap->GetWidth() - 1;
	_height = _heightMap->GetHeight() - 1;

	// Calculate the number of vertices in the terrain mesh.
	_vertexCount = _width * _height * 6;

	// Create the vertex array.
	_vertices = shared_ptr<TerrainVertex>(new TerrainVertex[_vertexCount]);

	this->LoadTextures(device, deviceContext, grassTexture, slopeTexture, rockTexture);

	// The terrain indicies list is always 0 -> vertexCount step 1
	_indices = shared_ptr<unsigned long>(new unsigned long[_vertexCount]);
    for (int i = 0; i < _vertexCount; ++i)
	{
		_indices.get()[i] = i;
	}

	this->Load(device, deviceContext);
}

void Terrain::Load(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
{
	this->CalculateTextureCoordinates();
	InitializeBuffers();

	LoadChunks(device);
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

	for (auto it = _chunks.begin(); it != _chunks.end(); ++it)
	{
		(*it).Draw(deviceContext);
	}
}

RayHit Terrain::IsRayIntersectingTerrain(Ray r)
{
	BoundingBox fakeBox;
	BoundingBox::CreateFromPoints(fakeBox, Vector3(0.0f, 0.0f, 0.0f), Vector3((float)_width, 0.0f, (float)_height));
	float distance_to_intersect = 0.0f;

	if (r.Intersects(fakeBox, distance_to_intersect))
	{
		cout << distance_to_intersect << endl;
		Vector3 hit_location = (r.position + (r.direction * distance_to_intersect));
		return RayHit(r.position, hit_location, distance_to_intersect, true);
	}

	return RayHit::NoHit;
}

int Terrain::GetVertexCount()
{
	return _vertexCount;
}

bool Terrain::InitializeBuffers()
{
	int index, i, j, index1, index2, index3, index4;
	float tu, tv;

	// Initialize the index to the vertex buffer.
	index = 0;

	auto p_vertices = _vertices.get();
	auto p_heightmap = _heightMap->GetHeightMapData().get();

	// Load the vertex and index array with the terrain data.
	for (j = 0; j < _height; j++)
	{
		for (i = 0; i < _width; i++)
		{
			index1 = ((int)_height * j) + i;          // Bottom left.
			index2 = ((int)_height * j) + (i + 1);      // Bottom right.
			index3 = ((int)_height * (j + 1)) + i;      // Upper left.
			index4 = ((int)_height * (j + 1)) + (i + 1);  // Upper right.

															 // Upper left.
			tv = p_heightmap[index3].tv;

			// Modify the texture coordinates to cover the top edge.
			if (tv == 1.0f) { tv = 0.0f; }

			p_vertices[index].position = Vector3(p_heightmap[index3].x, p_heightmap[index3].y,p_heightmap[index3].z);
			p_vertices[index].texture = Vector2(p_heightmap[index3].tu, tv);
			p_vertices[index].normal = Vector3(p_heightmap[index3].nx, p_heightmap[index3].ny, p_heightmap[index3].nz);

			index++;

			// Upper right.
			tu = p_heightmap[index4].tu;
			tv = p_heightmap[index4].tv;

			// Modify the texture coordinates to cover the top and right edge.
			if (tu == 0.0f) { tu = 1.0f; }
			if (tv == 1.0f) { tv = 0.0f; }

			p_vertices[index].position = Vector3(p_heightmap[index4].x, p_heightmap[index4].y, p_heightmap[index4].z);
			p_vertices[index].texture = Vector2(tu, tv);
			p_vertices[index].normal = Vector3(p_heightmap[index4].nx, p_heightmap[index4].ny, p_heightmap[index4].nz);
			index++;

			// Bottom left.
			p_vertices[index].position = Vector3(p_heightmap[index1].x, p_heightmap[index1].y, p_heightmap[index1].z);
			p_vertices[index].texture = Vector2(p_heightmap[index1].tu, p_heightmap[index1].tv);
			p_vertices[index].normal = Vector3(p_heightmap[index1].nx, p_heightmap[index1].ny, p_heightmap[index1].nz);
			index++;

			// Bottom left.
			p_vertices[index].position = Vector3(p_heightmap[index1].x, p_heightmap[index1].y, p_heightmap[index1].z);
			p_vertices[index].texture = Vector2(p_heightmap[index1].tu, p_heightmap[index1].tv);
			p_vertices[index].normal = Vector3(p_heightmap[index1].nx, p_heightmap[index1].ny, p_heightmap[index1].nz);
			index++;

			// Upper right.
			tu = p_heightmap[index4].tu;
			tv = p_heightmap[index4].tv;

			// Modify the texture coordinates to cover the top and right edge.
			if (tu == 0.0f) { tu = 1.0f; }
			if (tv == 1.0f) { tv = 0.0f; }

			p_vertices[index].position = Vector3(p_heightmap[index4].x, p_heightmap[index4].y, p_heightmap[index4].z);
			p_vertices[index].texture = Vector2(tu, tv);
			p_vertices[index].normal = Vector3(p_heightmap[index4].nx, p_heightmap[index4].ny, p_heightmap[index4].nz);
			index++;

			// Bottom right.
			tu = p_heightmap[index2].tu;

			// Modify the texture coordinates to cover the right edge.
			if (tu == 0.0f) { tu = 1.0f; }

			p_vertices[index].position = Vector3(p_heightmap[index2].x, p_heightmap[index2].y, p_heightmap[index2].z);
			p_vertices[index].texture = Vector2(tu, p_heightmap[index2].tv);
			p_vertices[index].normal = Vector3(p_heightmap[index2].nx, p_heightmap[index2].ny, p_heightmap[index2].nz);
			index++;
		}
	}

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


	// Calculate how much to increment the texture coordinates by.
	incrementValue = (float)TEXTURE_REPEAT / (float)_width;

	// Calculate how many times to repeat the texture.
	incrementCount = _width / TEXTURE_REPEAT;

	// Initialize the tu and tv coordinate values.
	tuCoordinate = 0.0f;
	tvCoordinate = 1.0f;

	// Initialize the tu and tv coordinate indexes.
	tuCount = 0;
	tvCount = 0;

	// Loop through the entire height map and calculate the tu and tv texture coordinates for each vertex.
	for (unsigned int j = 0; j < _height; j++)
	{
		for (unsigned int i = 0; i < _width; i++)
		{
			// Store the texture coordinate in the height map.
			_heightMap->GetHeightMapData().get()[(_height * j) + i].tu = tuCoordinate;
			_heightMap->GetHeightMapData().get()[(_height * j) + i].tv = tvCoordinate;

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

	unsigned long vert_index = 0;
	unsigned long chunk_vertex_index = 0;

	auto p_vertices = vertices.get();
	auto p__vertices = _vertices.get();
	auto p_indices = indices.get();

	for (int i = 0; i < _numChunksX; ++i)
	{
		for (int j = 0; j < _numChunksZ; ++j)
		{
			// for each chunk populate the arrays with necessary data

            for (int k = (i * _chunkWidth); k < (i * _chunkWidth) + _chunkWidth; ++k)
			{
                for (int l = (j * _chunkHeight); l < (j * _chunkHeight) + _chunkHeight; ++l)
				{
                 //   LOG_INFO(k, " ", l);
					// 6 vertices per cell
					for (int m = 0; m < 6; ++m)
					{
						p_vertices[chunk_vertex_index] = p__vertices[vert_index];
						p_indices[chunk_vertex_index] = chunk_vertex_index;
						chunk_vertex_index++;
						vert_index++;

					}
				}
			}

			_chunks.push_back(TerrainChunk(i * _chunkWidth, j * _chunkHeight, _chunkWidth, _chunkHeight, device, vertices, 64 * 6 * 64, indices));
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
	_heightMap->Add(location, radius, 550.0f, 750.0f);
	// internally the heightmap is  now updated with new values
	// Copy the new vertex data to the quadTree
	this->CalculateTextureCoordinates();
	InitializeBuffers();
	_chunks.clear();
	this->LoadChunks(device);
	_heightMap->Save();
}
