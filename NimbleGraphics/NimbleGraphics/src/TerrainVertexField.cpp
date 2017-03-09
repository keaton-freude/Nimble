#include "TerrainVertexField.h"
#include "Logger.h"

TerrainVertexField::TerrainVertexField(): _vertices(), width(0), height(0)
{
}

TerrainVertexField::TerrainVertexField(Dimension width, Dimension height, float resolution)
	: _vertices(width * height), width(width), height(height)
{
	for (Dimension j = 0; j < height; ++j)
	{
		for (Dimension i = 0; i < width; ++i)
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

Dimension TerrainVertexField::GetWidth() const
{
	return width;
}

Dimension TerrainVertexField::GetHeight() const
{
	return height;
}

TerrainVertex& TerrainVertexField::GetVert(int i, int j)
{
	auto index = j * height + i;
	return _vertices[index];
}

Dimension TerrainVertexField::GetVertIndex(int i, int j) const
{
	return j * height + i;
}
