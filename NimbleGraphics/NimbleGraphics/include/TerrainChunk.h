#pragma once
#include <memory>
#include "Transform.h"
#include "Structs.h"
#include "VertexBuffer.h"
#include "TerrainDrawable.h"
#include "TerrainVertexField.h"

class TerrainVertexField;
using std::shared_ptr;
using DirectX::BoundingBox;

class TerrainChunk
{
public:
	TerrainChunk(): _vertexCount(0), _positionX(0), _positionZ(0), _width(0), _height(0), chunk_x(0), chunk_z(0)
	{
	}

	TerrainChunk(int chunk_x, int chunk_z, unsigned int width, unsigned int height, ComPtr<ID3D11Device> device,
		vector<TerrainVertex>& vertices, vector<unsigned long>& indices)
		: _vertexCount(vertices.size()), _indexCount(indices.size()), _positionX(0), _positionZ(0), _width(width), _height(height),
		  _drawable(device, vertices, indices), _terrainCellData(width * height), chunk_x(chunk_x), chunk_z(chunk_z)
	{
		BoundingBox::CreateFromPoints(_boundingBox, Vector3(0.0f, 0.0f, 0.0f), Vector3(_width, 1.0f, height));
		auto vert_index = 0;

		//for(auto j = 0; j < height; ++j)
		//{
		//	for(auto i = 0; i < width; ++i)
		//	{
		//		// Copy the TerrainVertex data into our local _terrainCellData
		//		auto index = j * height + i;

		//		_terrainCellData[index].data.upperLeft = p_vertices[vert_index++];
		//		_terrainCellData[index].data.upperRight = p_vertices[vert_index++];
		//		_terrainCellData[index].data.bottomLeft = p_vertices[vert_index++];
		//		_terrainCellData[index].data.bottomLeft = p_vertices[vert_index++];
		//		_terrainCellData[index].data.upperRight = p_vertices[vert_index++];
		//		_terrainCellData[index].data.bottomRight = p_vertices[vert_index++];
		//	}
		//}
	}

	~TerrainChunk()
	{
		LOG_INFO("Terrain Chunk Destruct!");
	}

	void Draw(ComPtr<ID3D11DeviceContext> deviceContext)
	{
		_drawable.Draw(deviceContext, _indexCount);
	}

	vector<TerrainCell>& GetTerrainCellData()
	{
		return _terrainCellData;
	}

	void Update(ComPtr<ID3D11DeviceContext> deviceContext, vector<TerrainCell>& terrain_cells, unsigned int width, unsigned int height)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		TerrainVertex* dataPtr = nullptr;
		auto stride = sizeof(TerrainVertex);

		deviceContext->Map(_drawable.GetVertexBuffer()->GetVertexBuffer().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		dataPtr = static_cast<TerrainVertex*>(mappedResource.pData);

		auto vert_index = 0;

		for(auto j = 0; j < height; ++j)
		{
			for(auto i = 0; i < width; ++i)
			{
				auto index = j * height + i;

				auto& cell = terrain_cells[index];

				// We have 4 verts to update
				//dataPtr[vert_index++] = cell.data.upperLeft;
				//dataPtr[vert_index++] = cell.data.upperRight;
				//dataPtr[vert_index++] = cell.data.bottomLeft;
				//dataPtr[vert_index++] = cell.data.bottomLeft;
				//dataPtr[vert_index++] = cell.data.upperRight;
				//dataPtr[vert_index++] = cell.data.bottomRight;
			}
		}

		deviceContext->Unmap(_drawable.GetVertexBuffer()->GetVertexBuffer().Get(), 0);
	}

	void NewUpdate(ComPtr<ID3D11DeviceContext> deviceContext, TerrainVertexField* vertex_field)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		TerrainVertex* dataPtr = nullptr;
		auto stride = sizeof(TerrainVertex);

		auto p_field = vertex_field->GetVertices();

		deviceContext->Map(_drawable.GetVertexBuffer()->GetVertexBuffer().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		dataPtr = static_cast<TerrainVertex*>(mappedResource.pData);

		auto vert_index = 0;

		for (auto j = 0; j < (_height + 1); ++j)
		{
			for (auto i = 0; i < (_width + 1); ++i)
			{
				auto index = j * (_height + 1) + i;
				//index += (_width * chunk_x) + (chunk_z * vertex_field->GetHeight() * _height);
				dataPtr[vert_index++] = p_field[index];

			}
		}

		deviceContext->Unmap(_drawable.GetVertexBuffer()->GetVertexBuffer().Get(), 0);
	}

	BoundingBox GetBoundingBox() const { return _boundingBox; }
	Transform& transform() { return _drawable._transform; }
private:
	int _vertexCount;
	int _indexCount;
	int _positionX;
	int _positionZ;
	unsigned int _width;
	unsigned int _height;
	BoundingBox _boundingBox;
	TerrainDrawable _drawable;
	vector<TerrainCell> _terrainCellData;
	int chunk_x;
	int chunk_z;
};