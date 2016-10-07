#include "TerrainVertexField.h"

TerrainVertexField::TerrainVertexField(): _vertices(), width(0), height(0)
{
}

TerrainVertexField::TerrainVertexField(unsigned width, unsigned height, float resolution): _vertices(width * height), width(width), height(height)
{
	for (auto j = 0; j < height; ++j)
	{
		for (auto i = 0; i < width; ++i)
		{
			auto index = j * height + i;
			auto new_i = i * resolution;
			auto new_j = j * resolution;

			_vertices[index].position = Vector3(new_i, 0.0f, new_j);
			_vertices[index].normal = Vector3(0.01f, 1.0f, 0.01f);
			_vertices[index].texture = Vector2::Zero;
		}
	}
}

TerrainVertexField::~TerrainVertexField()
{
	LOG_INFO("Terrain Vertex Field destruct!");
}

std::vector<TerrainVertex>& TerrainVertexField::GetVertices()
{
	return _vertices;
}

unsigned TerrainVertexField::GetWidth() const
{
	return width;
}

unsigned TerrainVertexField::GetHeight() const
{
	return height;
}

TerrainVertex& TerrainVertexField::GetVert(int i, int j)
{
	auto index = j * height + i;
	return _vertices[index];
}

unsigned TerrainVertexField::GetVertIndex(int i, int j) const
{
	return j * height + i;
}
