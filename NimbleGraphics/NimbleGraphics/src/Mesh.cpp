#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

D3DBuffer Mesh::GetVertexBuffer() const
{
	return this->vertexBuffer;
}

D3DBuffer Mesh::GetIndexBuffer() const
{
	return this->indexBuffer;
}

UINT Mesh::GetNumVertices() const
{
	return meshData.numVertices;
}

UINT Mesh::GetNumIndices() const
{
	return meshData.numIndicies;
}

unique_ptr<Mesh> Mesh::CreateLine(D3DDevice device, Vector3 p1, Vector3 p2)
{
	auto vb = new Mesh();
	vb->LoadLine(device, p1, p2);

	return unique_ptr<Mesh>(vb);
}


shared_ptr<Mesh> Mesh::CreateDynamic(D3DDevice device, size_t vertexSize, size_t vertexCount, vector<unsigned long> indices, size_t index_count)
{
	Mesh* vb = new Mesh();
	vb->LoadDynamic(device, vertexCount, vertexSize, indices, index_count);
	return shared_ptr<Mesh>(vb);
}

void Mesh::LoadLine(D3DDevice device, Vector3 p1, Vector3 p2)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;


	meshData.numIndicies = 2;
	meshData.numVertices = 2;

	auto indices = unique_ptr<unsigned long>(new unsigned long[2]);
	indices.get()[0] = 0;
	indices.get()[1] = 1;

	auto vertices = unique_ptr<ColorVertex>(new ColorVertex[2]);

	vertices.get()[0].position = p1;
	vertices.get()[0].color = DirectX::Colors::White.v;//Color(1.0f, 0.0f, 0.0f);

	vertices.get()[1].position = p2;
	vertices.get()[1].color = DirectX::Colors::Green.v;// Color(1.0f, 0.0f, 0.0f);

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(ColorVertex) * 2;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices.get();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;


	device->CreateBuffer(&vertexBufferDesc, &vertexData, vertexBuffer.GetAddressOf());

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
	device->CreateBuffer(&indexBufferDesc, &indexData, indexBuffer.GetAddressOf());
}

void Mesh::LoadDynamic(D3DDevice device, size_t vertexSize, size_t vertexCount, vector<unsigned long> indices, size_t index_count)
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
