#pragma once
#include <vector>
#include "Structs.h"

/* This class represents the actual vertices for a terrain.
 * The terrain class and other things based on this, create views
 * of this underlying vertex field.
 * 
 * Other classes or tools will access this directly. When adding
 * to the heightmap via a tool, we will modify the underlying
 * Vertex field. Terrain chunks just index the vertices in a 
 * way that the GPU understands.
 */

class TerrainVertexField
{
public:
	TerrainVertexField()
		: _vertices(), width(0), height(0)
	{
	}

	TerrainVertexField(unsigned int width, unsigned int height, float resolution)
		: _vertices(width * height), width(width), height(height)
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

	~TerrainVertexField()
	{
		LOG_INFO("Terrain Vertex Field destruct!");
	}

	vector<TerrainVertex>& GetVertices()
	{
		return _vertices;
	}

	unsigned int GetWidth() const
	{
		return width;
	}

	unsigned int GetHeight() const
	{
		return height;
	}

	TerrainVertex& GetVert(int i, int j)
	{
		auto index = j * height + i;
		return _vertices[index];
	}

	unsigned int GetVertIndex(int i, int j) const
	{
		return j * height + i;
	}

private:
	std::vector<TerrainVertex> _vertices;
	unsigned int width;
	unsigned int height;
};