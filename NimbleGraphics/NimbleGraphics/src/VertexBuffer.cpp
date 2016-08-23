#include "VertexBuffer.h"
#include <vector>
#include "ColorShader.h"
#include <DirectXColors.h>
#include <iostream>
#include "Logger.h"

using namespace std;

using namespace DirectX::Colors;

VertexBuffer::VertexBuffer()
{
}

VertexBuffer::~VertexBuffer()
{
	LOG_INFO("Vertex Buffer destruct!");
	this->Unload();
}

bool VertexBuffer::Load(ComPtr<ID3D11Device> device)
{
	// nothing to load
	return true;
}

void VertexBuffer::LoadLine(ComPtr<ID3D11Device> device, Vector3 p1, Vector3 p2)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;


	meshData.numIndicies = 2;
	meshData.numVertices = 2;

	auto indices = unique_ptr<unsigned long>(new unsigned long[2]);
	indices.get()[0] = 0;
	indices.get()[1] = 1;

	auto vertices = unique_ptr<ColorVertex>(new ColorVertex[2]);

	vertices.get()[0].position = p1;
	vertices.get()[0].color = White.v;//Color(1.0f, 0.0f, 0.0f);

	vertices.get()[1].position = p2;
	vertices.get()[1].color = Green.v;// Color(1.0f, 0.0f, 0.0f);

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(ColorVertex) * 2;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices.get();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;


	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, vertexBuffer.GetAddressOf());

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * this->meshData.numIndicies;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices.get();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;


	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, indexBuffer.GetAddressOf());
}

// Create a 
void VertexBuffer::LoadTerrain(ComPtr<ID3D11Device> device, UINT width, UINT height, float resolution)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	
	unique_ptr<ColorVertex> vertices;
	unique_ptr<unsigned long> indices;
	int index;
	float posX, posZ;

	// naive approach, can improve by removing non-unique vertices
	this->meshData.numVertices = (width - 1) * (height - 1) * 8;
	// again, naive approach. will improve after working
	this->meshData.numIndicies = this->meshData.numVertices;

	vertices = unique_ptr<ColorVertex>(new ColorVertex[this->meshData.numVertices]);
	indices = unique_ptr<unsigned long>(new unsigned long[this->meshData.numIndicies]);

	index = 0;

	for (UINT j = 0; j < (height - 1); ++j)
	{
		for (UINT i = 0; i < (width - 1); ++i)
		{
			// Line 1
			// Top-Left
			posX = i * resolution;
			posZ = (float)(j * resolution + resolution);

			vertices.get()[index].position = Vector3(posX, 0.0f, posZ);
			vertices.get()[index].color = White.v;
			indices.get()[index] = index;
			index++;

			// Top-Right
			posX = (float)(i * resolution + resolution);
			posZ = (float)(j * resolution + resolution);

			vertices.get()[index].position = Vector3(posX, 0.0f, posZ);
			vertices.get()[index].color = White.v;
			indices.get()[index] = index;
			index++;

			// Line 2
			// Top-Right
			posX = (float)(i * resolution + resolution);
			posZ = (float)(j * resolution + resolution);

			vertices.get()[index].position = Vector3(posX, 0.0f, posZ);
			vertices.get()[index].color = White.v;
			indices.get()[index] = index;
			index++;

			// Bottom-Right
			posX = (float)(i * resolution + resolution);
			posZ = (float)j * resolution;

			vertices.get()[index].position = Vector3(posX, 0.0f, posZ);
			vertices.get()[index].color = White.v;
			indices.get()[index] = index;
			index++;

			// Line 3
			// Bottom-Right
			posX = (float)(i * resolution + resolution);
			posZ = (float)j * resolution;

			vertices.get()[index].position = Vector3(posX, 0.0f, posZ);
			vertices.get()[index].color = White.v;
			indices.get()[index] = index;
			index++;

			// Bottom-Left
			posX = (float)i * resolution;
			posZ = (float)j * resolution;

			vertices.get()[index].position = Vector3(posX, 0.0f, posZ);
			vertices.get()[index].color = White.v;
			indices.get()[index] = index;
			index++;

			// Line 4
			// Bottom-Left
			posX = (float)i * resolution;
			posZ = (float)j * resolution;

			vertices.get()[index].position = Vector3(posX, 0.0f, posZ);
			vertices.get()[index].color = White.v;
			indices.get()[index] = index;
			index++;

			// Upper-left
			posX = (float)i * resolution;
			posZ = (float)(j * resolution + resolution);

			vertices.get()[index].position = Vector3(posX, 0.0f, posZ);
			vertices.get()[index].color = White.v;
			indices.get()[index] = index;
			index++;

		}
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(ColorVertex) * this->meshData.numVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices.get();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, vertexBuffer.GetAddressOf());

	if (FAILED(result))
	{
		cout << "Vertex buffer FAILED to create." << endl;
		return;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * this->meshData.numIndicies;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices.get();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, indexBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return;
	}
}

void VertexBuffer::Unload()
{
	
}

ComPtr<ID3D11Buffer> VertexBuffer::GetVertexBuffer()
{
	 return this->vertexBuffer;
}

ComPtr<ID3D11Buffer> VertexBuffer::GetIndexBuffer()
{
	 return this->indexBuffer;
}

UINT VertexBuffer::GetNumVertices()
{
	return meshData.numVertices;
}

UINT VertexBuffer::GetNumIndices()
{
	return meshData.numIndicies;
}

void VertexBuffer::LoadFromHeightmap(ComPtr<ID3D11Device> device,
	shared_ptr<TerrainVertex> heightMap, unsigned int width, unsigned int height)
{
	unique_ptr<TerrainVertex> vertices;
	unique_ptr<unsigned long> indices;

	unsigned int index, i, j;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int index1, index2, index3, index4;
	float tu, tv;

	meshData.numVertices = (width - 1) * (height - 1) * 6;
	meshData.numIndicies = meshData.numVertices;

	vertices = unique_ptr<TerrainVertex>(new TerrainVertex[meshData.numVertices]);

	indices = unique_ptr<unsigned long>(new unsigned long[meshData.numIndicies]);

	index = 0;

	auto heightMapPtr = heightMap.get();

	// Load the vertex and index array with the terrain data.
	for (j = 0; j < (height - 1); j++)
	{
		for (i = 0; i<(width - 1); i++)
		{
			index1 = (height * j) + i;          // Bottom left.
			index2 = (height * j) + (i + 1);      // Bottom right.
			index3 = (height * (j + 1)) + i;      // Upper left.
			index4 = (height * (j + 1)) + (i + 1);  // Upper right.

															 // Upper left.
			tv = heightMapPtr[index3].texture.y;

			// Modify the texture coordinates to cover the top edge.
			if (tv == 1.0f) { tv = 0.0f; }

			vertices.get()[index].position = heightMapPtr[index3].position;
			vertices.get()[index].texture = heightMapPtr[index3].texture;
			vertices.get()[index].normal = heightMapPtr[index3].normal;
			indices.get()[index] = index;
			index++;

			// Upper right.
			tu = heightMapPtr[index4].texture.x;
			tv = heightMapPtr[index4].texture.y;

			// Modify the texture coordinates to cover the top and right edge.
			if (tu == 0.0f) { tu = 1.0f; }
			if (tv == 1.0f) { tv = 0.0f; }

			vertices.get()[index].position = heightMapPtr[index4].position;
			vertices.get()[index].texture = Vector2(tu, tv);
			vertices.get()[index].normal = heightMapPtr[index4].normal;
			indices.get()[index] = index;
			index++;

			// Bottom left.
			vertices.get()[index].position = heightMapPtr[index1].position;
			vertices.get()[index].texture = heightMapPtr[index1].texture;
			vertices.get()[index].normal = heightMapPtr[index1].normal;
			indices.get()[index] = index;
			index++;

			// Bottom left.
			vertices.get()[index].position = heightMapPtr[index1].position;
			vertices.get()[index].texture = heightMapPtr[index1].texture;
			vertices.get()[index].normal = heightMapPtr[index1].normal;
			indices.get()[index] = index;
			index++;

			// Upper right.
			tu = heightMapPtr[index4].texture.x;
			tv = heightMapPtr[index4].texture.y;

			// Modify the texture coordinates to cover the top and right edge.
			if (tu == 0.0f) { tu = 1.0f; }
			if (tv == 1.0f) { tv = 0.0f; }

			vertices.get()[index].position = heightMapPtr[index4].position;
			vertices.get()[index].texture = Vector2(tu, tv);
			vertices.get()[index].normal = heightMapPtr[index4].normal;
			indices.get()[index] = index;
			index++;

			// Bottom right.
			tu = heightMapPtr[index2].texture.x;

			// Modify the texture coordinates to cover the right edge.
			if (tu == 0.0f) { tu = 1.0f; }

			vertices.get()[index].position = heightMapPtr[index2].position;
			vertices.get()[index].texture = Vector2(tu, heightMapPtr[index2].texture.y);
			vertices.get()[index].normal = heightMapPtr[index2].normal;
			indices.get()[index] = index;
			index++;
		}
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(TerrainVertex) * meshData.numVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices.get();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// D3D requires us to create the buffer on a raw pointer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, vertexBuffer.GetAddressOf());

	if (FAILED(result))
	{
		cout << "Failed to create Terrain Vertex Buffer." << endl;
	}
	
	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * meshData.numIndicies;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices.get();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, indexBuffer.GetAddressOf());

	if (FAILED(result))
	{
		cout << "Failed to create Terrain Index Buffer." << endl;
	}
}

void VertexBuffer::LoadTerrainNode(ComPtr<ID3D11Device> device, shared_ptr<TerrainVertex> vertices, 
	int vertexCount, shared_ptr<unsigned long> indicies)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set up the description of the vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC; // <-- This allows us to make write changes during runtime
	vertexBufferDesc.ByteWidth = sizeof(TerrainVertex) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices.get();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now finally create the vertex buffer.
	device->CreateBuffer(&vertexBufferDesc, &vertexData, vertexBuffer.GetAddressOf());

	// Set up the description of the index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * vertexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indicies.get();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	device->CreateBuffer(&indexBufferDesc, &indexData, indexBuffer.GetAddressOf());
}

void VertexBuffer::LoadDynamic(ComPtr<ID3D11Device> device, size_t vertexSize, size_t vertexCount, vector<unsigned long> indices, size_t index_count)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;

	// Set up the description of the vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC; // <-- This allows us to make write changes during runtime
	vertexBufferDesc.ByteWidth = vertexSize * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	device->CreateBuffer(&vertexBufferDesc, nullptr, vertexBuffer.GetAddressOf());

	// Set up the description of the index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * index_count;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = &indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	device->CreateBuffer(&indexBufferDesc, &indexData, indexBuffer.GetAddressOf());
}


shared_ptr<VertexBuffer> VertexBuffer::CreateLine(ComPtr<ID3D11Device> device, Vector3 p1, Vector3 p2)
{
	VertexBuffer* vb = new VertexBuffer();
	vb->LoadLine(device, p1, p2);

	return shared_ptr<VertexBuffer>(vb);
}

shared_ptr<VertexBuffer> VertexBuffer::CreateFromHeightMap(ComPtr<ID3D11Device> device,
	shared_ptr<TerrainVertex> heightMap, unsigned int width, unsigned int height)
{
	VertexBuffer* vb = new VertexBuffer();
	vb->LoadFromHeightmap(device, heightMap, width, height);
	
	return shared_ptr<VertexBuffer>(vb);
}

shared_ptr<VertexBuffer> VertexBuffer::CreateTerrain(ComPtr<ID3D11Device> device,
	UINT width, UINT height, float resolution)
{
	VertexBuffer* vb = new VertexBuffer();
	vb->LoadTerrain(device, width, height, resolution);

	return shared_ptr<VertexBuffer>(vb);
}

shared_ptr<VertexBuffer> VertexBuffer::CreateTerrainNode(ComPtr<ID3D11Device> device, shared_ptr<TerrainVertex> vertices, int vertexCount, shared_ptr<unsigned long> indices)
{
	VertexBuffer* vb = new VertexBuffer();
	vb->LoadTerrainNode(device, vertices, vertexCount, indices);
	return shared_ptr<VertexBuffer>(vb);
}

shared_ptr<VertexBuffer> VertexBuffer::CreateDynamic(ComPtr<ID3D11Device> device, size_t vertexSize, size_t vertexCount, vector<unsigned long> indices, size_t index_count)
{
	VertexBuffer* vb = new VertexBuffer();
	vb->LoadDynamic(device, vertexCount, vertexSize, indices, index_count);
	return shared_ptr<VertexBuffer>(vb);
}
