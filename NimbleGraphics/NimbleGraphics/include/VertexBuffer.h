#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include <memory>
#include <vector>

#include "IResource.h"
#include "Structs.h"


#include <wrl/client.h>
#include <DirectXColors.h>

using DirectX::SimpleMath::Vector3;
using std::shared_ptr;
using Microsoft::WRL::ComPtr;
using std::vector;

class VertexBuffer : public IResource
{
public:
	VertexBuffer()
	{
	}

	~VertexBuffer()
	{
		//LOG_INFO("Vertex Buffer destruct!");
		this->VertexBuffer::Unload();
	}

	// IResource Load
	bool Load(ComPtr<ID3D11Device> device) override
	{
		// nothing to load
		return true;
	}

	// IResource Unload
	void Unload() override
	{
	}

	ComPtr<ID3D11Buffer> GetVertexBuffer() const
	{
		return this->vertexBuffer;
	}

	ComPtr<ID3D11Buffer> GetIndexBuffer() const
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
	static shared_ptr<VertexBuffer> CreateLine(ComPtr<ID3D11Device> device, Vector3 p1, Vector3 p2)
	{
		VertexBuffer* vb = new VertexBuffer();
		vb->LoadLine(device, p1, p2);

		return shared_ptr<VertexBuffer>(vb);
	}

	static shared_ptr<VertexBuffer> CreateTerrainNode(ComPtr<ID3D11Device> device,
		vector<TerrainVertex>& vertices, vector<unsigned long>& indices)
	{
		VertexBuffer* vb = new VertexBuffer();
		vb->LoadTerrainNode(device, vertices, indices);
		return shared_ptr<VertexBuffer>(vb);
	}

	static shared_ptr<VertexBuffer> CreateDynamic(ComPtr<ID3D11Device> device, size_t vertexSize, size_t vertexCount,
	                                              vector<unsigned long> indices, size_t index_count)
	{
		VertexBuffer* vb = new VertexBuffer();
		vb->LoadDynamic(device, vertexCount, vertexSize, indices, index_count);
		return shared_ptr<VertexBuffer>(vb);
	}

	void LoadLine(ComPtr<ID3D11Device> device, Vector3 p1, Vector3 p2)
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

	void LoadTerrainNode(ComPtr<ID3D11Device> device, vector<TerrainVertex>& vertices, vector<unsigned long>& indicies)
	{
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		HRESULT result;

		// Set up the description of the vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC; // <-- This allows us to make write changes during runtime
		vertexBufferDesc.ByteWidth = sizeof(TerrainVertex) * vertices.size();
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

	void LoadDynamic(ComPtr<ID3D11Device> device, size_t vertexSize, size_t vertexCount,
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
	void SetData(ComPtr<ID3D11DeviceContext> deviceContext, T* data, int indexOffset, int numberElements)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		T* dataPtr = nullptr;
		auto stride = sizeof(T);


		deviceContext->Map(this->vertexBuffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource);

		dataPtr = static_cast<T*>(mappedResource.pData);

		memcpy(&dataPtr[indexOffset], &data[indexOffset], numberElements * stride);

		deviceContext->Unmap(this->vertexBuffer.Get(), 0);
	}


private:
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	MeshData meshData;
};
