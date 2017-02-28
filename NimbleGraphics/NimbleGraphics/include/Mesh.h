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
	Mesh()
	{
		
	}

	Mesh(const Mesh& other)
	{
		
	}

	Mesh& operator=(Mesh&&) = default;


	D3DBuffer GetVertexBuffer() const
	{
		return this->vertexBuffer;
	}

	D3DBuffer GetIndexBuffer() const
	{
		return this->indexBuffer;
	}

	UINT GetNumVertices() const
	{
		return meshData.numVertices;
	}

	UINT GetNumIndices() const
	{
		return meshData.numIndicies;
	}

	// Factory Methods - use these for object creation
	static unique_ptr<Mesh> CreateLine(D3DDevice device, Vector3 p1, Vector3 p2)
	{
		auto vb = new Mesh();
		vb->LoadLine(device, p1, p2);

		return unique_ptr<Mesh>(vb);
	}

	template<typename T>
	static unique_ptr<Mesh> Create(D3DDevice device, vector<T>& vertices, vector<unsigned long>& indices, bool dynamic)
	{
		auto vb = new Mesh();
		vb->Load<T>(device, vertices, indices, dynamic);
		return unique_ptr<Mesh>(vb);
	}

	static shared_ptr<Mesh> CreateDynamic(D3DDevice device, size_t vertexSize, size_t vertexCount,
	                                      vector<unsigned long> indices, size_t index_count)
	{
		Mesh* vb = new Mesh();
		vb->LoadDynamic(device, vertexCount, vertexSize, indices, index_count);
		return shared_ptr<Mesh>(vb);
	}

	void LoadLine(D3DDevice device, Vector3 p1, Vector3 p2)
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

	static shared_ptr<Mesh> CreateFromHeightmap(D3DDevice device, MemoryHeightmap& mem_heightmap)
	{
		auto vb = new Mesh();
		vb->LoadFromHeightmap(device, mem_heightmap);
		return shared_ptr<Mesh>(vb);
	}

	void LoadFromHeightmap(D3DDevice device, MemoryHeightmap& mem_heightmap)
	{
		vector<TerrainVertex> vertices((mem_heightmap.GetWidth() + 1) * (mem_heightmap.GetHeight() + 1));
		vector<unsigned long> indices(mem_heightmap.GetWidth() * 6 * mem_heightmap.GetHeight());
		//_positionVertices.reserve((mem_heightmap.GetWidth() + 1) * (mem_heightmap.GetHeight() + 1));
		_triangles.resize(mem_heightmap.GetWidth() * mem_heightmap.GetHeight() * 2);
		unsigned int index = 0;
		auto width = mem_heightmap.GetWidth();
		auto height = mem_heightmap.GetHeight();
		auto p_map = mem_heightmap.GetVertexField()->GetVertices();

		// indices
		for (auto j = 0; j < height; ++j)
		{
			for (auto i = 0; i < width; ++i)
			{
				unsigned int bottomLeft = (j * (height + 1)) + i;
				unsigned int bottomRight = (j * (height + 1)) + (i + 1);
				unsigned int topLeft = ((j + 1) * (height + 1)) + i;
				unsigned int topRight = ((j + 1) * (height + 1)) + (i + 1);

				//upperLeft
				indices[index++] = topLeft;
				indices[index++] = topRight;
				indices[index++] = bottomLeft;

				indices[index++] = bottomLeft;
				indices[index++] = topRight;
				indices[index++] = bottomRight;
			}
		}

		Load<TerrainVertex>(device, mem_heightmap.GetVertexField()->GetVertices(), indices, true);

		//unsigned int i = 0;
		//for (const auto& vert : mem_heightmap.GetVertexField()->GetVertices())
		//{
		//	_positionVertices[i++].position = vert.position;
		//}
		auto& data = mem_heightmap.GetHeightmapData();

		index = 0;
		for (auto j = 0; j < height; ++j)
		{
			for (auto i = 0; i < width; ++i)
			{
				auto& cell = data[j * height + i];

				_triangles[index++] = Triangle(cell.data.upperLeft.position, cell.data.upperRight.position, cell.data.bottomLeft.position);
				_triangles[index++] = Triangle(cell.data.bottomLeft.position, cell.data.upperRight.position, cell.data.bottomRight.position);
			}
		}
	}

	template<typename T>
	void Load(D3DDevice device, vector<T>& vertices, vector<unsigned long>& indicies, bool dynamic)
	{
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		HRESULT result;

		meshData.stride = sizeof(T);
		meshData.numVertices = vertices.size();
		meshData.numIndicies = indicies.size();

		auto usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;

		// Set up the description of the vertex buffer.
		vertexBufferDesc.Usage = usage; 
		vertexBufferDesc.ByteWidth = sizeof(T) * vertices.size();
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
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * indicies.size();
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

	void LoadDynamic(D3DDevice device, size_t vertexSize, size_t vertexCount,
	                 vector<unsigned long> indices, size_t index_count)
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

	// XNA-style vertexbuffer updates
	// provide the baseOffset (for @vertices), pointer to CPU buffer
	// indexOffset (for buffer), numberElements (amount of verts to update),
	// stride (size of each element)
	template<typename T>
	void SetData(D3DDeviceContext deviceContext, T* data, int indexOffset, int numberElements)
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
	void SetDataRegion(D3DDeviceContext deviceContext, unsigned int x, unsigned int y, size_t width, size_t height, T* data)
	{
		D3D11_MAPPED_SUBRESOURCE resource;

		deviceContext->Map(this->vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		auto dataPtr = static_cast<T*>(resource.pData);

		for (auto iy = y; iy < height; ++iy)
		{
			for(auto ix = x; ix < width; ++ix)
			{
				dataPtr[iy * width + ix] = data[iy * width + ix];
			}
		}

		deviceContext->Unmap(vertexBuffer.Get(), 0);
	}

	const MeshData& GetMeshData() const
	{
		return meshData;
	}

	const vector<Triangle>& GetTriangles() const
	{
		return _triangles;
	}

private:
	vector<Triangle> _triangles;
	D3DBuffer vertexBuffer;
	D3DBuffer indexBuffer;
	MeshData meshData;
};
