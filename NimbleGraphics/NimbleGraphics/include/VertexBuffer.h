#pragma once

#include <d3d11.h>
#include <stdio.h>
#include <SimpleMath.h>
#include <memory>

#include "IResource.h"
#include "Heightmap.h"
#include "Structs.h"


#include <wrl\client.h>

using DirectX::SimpleMath::Vector3;
using std::shared_ptr;
using Microsoft::WRL::ComPtr;

class VertexBuffer : public IResource
{
public:
	VertexBuffer();
	~VertexBuffer();

	// IResource Load
	bool Load(ComPtr<ID3D11Device> device);

	// IResource Unload
	void Unload();

	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer();
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer();
	UINT GetNumVertices();
	UINT GetNumIndices();

	// Factory Methods - use these for object creation
	static shared_ptr<VertexBuffer> CreateLine(ComPtr<ID3D11Device> device, Vector3 p1, Vector3 p2);
	static shared_ptr<VertexBuffer> CreateFromHeightMap(ComPtr<ID3D11Device> device,
		shared_ptr<HeightMapData> heightMap, unsigned int width, unsigned int height);
	static shared_ptr<VertexBuffer> CreateTerrain(ComPtr<ID3D11Device> device,
		UINT width, UINT height, float resolution);
	static shared_ptr<VertexBuffer> CreateTerrainNode(ComPtr<ID3D11Device> device,
		shared_ptr<TerrainVertex> vertices, int vertexCount, shared_ptr<unsigned long> indices);
	static shared_ptr<VertexBuffer> CreateDynamic(ComPtr<ID3D11Device> device, size_t vertexSize, size_t vertexCount,
	                                              vector<unsigned long> indices, size_t index_count);


	// Load methods, populate the class with the correct values
	void LoadFromHeightmap(ComPtr<ID3D11Device> device, shared_ptr<HeightMapData> heightMap,
		unsigned int width, unsigned int height);
	void LoadTerrain(ComPtr<ID3D11Device> device, UINT width, UINT height, float resolution);
	void LoadLine(ComPtr<ID3D11Device> device, Vector3 p1, Vector3 p2);
	void LoadTerrainNode(ComPtr<ID3D11Device> device, shared_ptr<TerrainVertex> vertices, int vertexCount,
		shared_ptr<unsigned long> indicies);
	void LoadDynamic(ComPtr<ID3D11Device> device, size_t vertexSize, size_t vertexCount,
	                 vector<unsigned long> indices, size_t index_count);

	// XNA-style vertexbuffer updates
	// provide the baseOffset (for @vertices), pointer to CPU buffer
	// indexOffset (for buffer), numberElements (amount of verts to update),
	// stride (size of each element)

	template<typename T>
	void SetData(ComPtr<ID3D11DeviceContext> deviceContext, T* data, int indexOffset, int numberElements);


private:
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	MeshData meshData;
};

template<typename T>
inline void VertexBuffer::SetData(ComPtr<ID3D11DeviceContext> deviceContext, T * data, int indexOffset, int numberElements)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	T* dataPtr = nullptr;
	auto stride = sizeof(T);
	

	deviceContext->Map(this->vertexBuffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource);

	dataPtr = static_cast<T*>(mappedResource.pData);
	
	memcpy(&dataPtr[indexOffset], &data[indexOffset], numberElements * stride);

	deviceContext->Unmap(this->vertexBuffer.Get(), 0);
}
