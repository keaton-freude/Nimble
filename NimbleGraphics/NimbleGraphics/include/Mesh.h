#pragma once
#include "Typedefs.h"
#include "Structs.h"
#include <memory>
#include <vector>
#include <wrl/client.h>
#include <DirectXColors.h>
#include "MemoryHeightmap.h"

using DirectX::SimpleMath::Vector3;
using std::shared_ptr;
using Microsoft::WRL::ComPtr;
using std::vector;

class Mesh
{
public:
	Mesh();
	Mesh(const Mesh& other);
	Mesh& operator=(Mesh&&) = default;
	D3DBuffer GetVertexBuffer() const;
	D3DBuffer GetIndexBuffer() const;
	UINT GetNumVertices() const;
	UINT GetNumIndices() const;

	// Factory Methods - use these for object creation
	static unique_ptr<Mesh> CreateLine(D3DDevice device, Vector3 p1, Vector3 p2);

	template<typename T>
	static unique_ptr<Mesh> Create(D3DDevice device, vector<T>& vertices, vector<unsigned long>& indices, bool dynamic);
	static shared_ptr<Mesh> CreateDynamic(D3DDevice device, size_t vertexSize, size_t vertexCount,
		vector<unsigned long> indices, size_t index_count);
	void LoadLine(D3DDevice device, Vector3 p1, Vector3 p2);

	static shared_ptr<Mesh> CreateFromHeightmap(D3DDevice device, MemoryHeightmap& mem_heightmap);
	void LoadFromHeightmap(D3DDevice device, MemoryHeightmap& mem_heightmap);

	template<typename T>
	void Load(D3DDevice device, vector<T>& vertices, vector<unsigned long>& indicies, bool dynamic);


	// Note: The vertexSize * vertexCount must not exceed sizeof(UINT)
	void LoadDynamic(D3DDevice device, size_t vertexSize, size_t vertexCount,
		vector<unsigned long> indices, size_t index_count);

	// XNA-style vertexbuffer updates
	// provide the baseOffset (for @vertices), pointer to CPU buffer
	// indexOffset (for buffer), numberElements (amount of verts to update),
	// stride (size of each element)
	template<typename T>
	void SetData(D3DDeviceContext deviceContext, T* data, int indexOffset, int numberElements);

	template<typename T>
	void SetDataRegion(D3DDeviceContext deviceContext, unsigned int x, unsigned int y, size_t width, size_t height, T* data);

	const MeshData& GetMeshData() const;
	const vector<Triangle>& GetTriangles() const;

private:
	vector<Triangle> _triangles;

	D3DBuffer vertexBuffer;
	D3DBuffer indexBuffer;
	MeshData meshData;
};


template<typename T>
unique_ptr<Mesh> Mesh::Create(D3DDevice device, vector<T>& vertices, vector<unsigned long>& indices, bool dynamic)
{
	auto vb = new Mesh();
	vb->Load<T>(device, vertices, indices, dynamic);
	return unique_ptr<Mesh>(vb);
}

template<typename T>
void Mesh::Load(D3DDevice device, vector<T>& vertices, vector<unsigned long>& indicies, bool dynamic)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	meshData.stride = sizeof(T);
	meshData.numVertices = static_cast<UINT>(vertices.size());
	meshData.numIndicies = static_cast<UINT>(indicies.size());

	auto usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;

	// Set up the description of the vertex buffer.
	vertexBufferDesc.Usage = usage;
	vertexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(T) * vertices.size());
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = &vertices[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now finally create the vertex buffer.
	device->CreateBuffer(&vertexBufferDesc, &vertexData, vertexBuffer.GetAddressOf());

	// Set up the description of the index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(unsigned long) * indicies.size());
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = &indicies[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	device->CreateBuffer(&indexBufferDesc, &indexData, indexBuffer.GetAddressOf());
}

// XNA-style vertexbuffer updates
// provide the baseOffset (for @vertices), pointer to CPU buffer
// indexOffset (for buffer), numberElements (amount of verts to update),
// stride (size of each element)
template<typename T>
void Mesh::SetData(D3DDeviceContext deviceContext, T* data, int indexOffset, int numberElements)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	T* dataPtr = nullptr;
	auto stride = sizeof(T);

	deviceContext->Map(this->vertexBuffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource);

	dataPtr = static_cast<T*>(mappedResource.pData);

	memcpy(&dataPtr[indexOffset], &data[indexOffset], numberElements * stride);

	deviceContext->Unmap(this->vertexBuffer.Get(), 0);
}

template<typename T>
void Mesh::SetDataRegion(D3DDeviceContext deviceContext, unsigned int x, unsigned int y, size_t width, size_t height, T* data)
{
	D3D11_MAPPED_SUBRESOURCE resource;

	deviceContext->Map(this->vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	auto dataPtr = static_cast<T*>(resource.pData);

	for (auto iy = y; iy < height; ++iy)
	{
		for (auto ix = x; ix < width; ++ix)
		{
			dataPtr[iy * width + ix] = data[iy * width + ix];
		}
	}

	deviceContext->Unmap(vertexBuffer.Get(), 0);
}
