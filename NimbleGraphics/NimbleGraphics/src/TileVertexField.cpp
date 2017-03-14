#include "TileVertexField.h"
#include "Logger.h"

TileVertexField::TileVertexField()
	: _vertices(), width(0), height(0)
{
}

TileVertexField::TileVertexField(Dimension width, Dimension height, float resolution)
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

TileVertexField::~TileVertexField()
{
	LOG_INFO("Terrain Vertex Field destruct!");
}

std::vector<TileVertex>& TileVertexField::GetVertices()
{
	return _vertices;
}

Dimension TileVertexField::GetWidth() const
{
	return width;
}

Dimension TileVertexField::GetHeight() const
{
	return height;
}

TileVertex& TileVertexField::GetVert(int i, int j)
{
	auto index = j * height + i;
	return _vertices[index];
}

Dimension TileVertexField::GetVertIndex(int i, int j) const
{
	return j * height + i;
}
