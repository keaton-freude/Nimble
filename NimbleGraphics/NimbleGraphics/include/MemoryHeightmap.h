#pragma once
#include "Helper.h"
#include <vector>
#include <fstream>
#include "Structs.h"
#include "DebugLineManager.h"
#include "TerrainVertexField.h"

namespace DirectX{
	namespace SimpleMath{
		struct Vector3;
	}
}

class MemoryHeightmap
{
public:
	MemoryHeightmap()
		: highest_point(0.0f), _heightmap(), _width(0), _height(0), _vertex_field()
	{
		
	}

	~MemoryHeightmap()
	{
		
	}

	MemoryHeightmap(unsigned int width, unsigned int height, float resolution = 1.0f)
		: highest_point(0.0f), _width(width), _height(height), _heightmap(width * height), _vertex_field(width + 1, height + 1, resolution)
	{
		auto& p_verts = _vertex_field.GetVertices();
		for(unsigned int j = 0; j < _height; ++j)
		{
			for(unsigned int i = 0; i < _width; ++i)
			{
				auto vf_height = _vertex_field.GetHeight();
				auto index = j * _height + i;

				auto blIndex = j * vf_height + i;
				auto brIndex = j * vf_height + (i + 1);
				auto ulIndex = (j + 1) * vf_height + i;
				auto urIndex = (j + 1) * vf_height + (i + 1);

				// Create _symbolic_ links to the underlying verts				
				_heightmap[index].data.upperLeft = &p_verts[ulIndex];
				_heightmap[index].data.upperRight = &p_verts[urIndex];
				_heightmap[index].data.bottomLeft = &p_verts[blIndex];
				_heightmap[index].data.bottomRight = &p_verts[brIndex];
			}
		}

		CalculateTextureCoordinates();
	}

	void CalculateTextureCoordinates()
	{
		int incrementCount, tuCount, tvCount;
		float incrementValue, tuCoordinate, tvCoordinate;

		incrementValue = (float)TEXTURE_REPEAT / _width;

		float tu, tv;

		tv = 1.0f;
		tu = 0.0f;

		auto vf_height = _vertex_field.GetHeight();
		auto vf_width = _vertex_field.GetWidth();

		incrementCount = _width / TEXTURE_REPEAT;
		auto& p_vertex_field = GetVertexField()->GetVertices();

		for (unsigned int j = 0; j < vf_height; ++j)
		{
			for (unsigned int i = 0; i < vf_width; ++i)
			{
				auto index = j * vf_height + i;

				// We are moving left to right, bottom to top
				// each time we move right, we need to
				// 1) write our current value, then increment
				p_vertex_field[index].texture = Vector2(tu, tv);
				tu += incrementValue;
			}
			tu = 0.0f;
			tv -= incrementValue;
		}
	}

	void SmoothAdd(DirectX::SimpleMath::Vector3 location, float radius, float intensity)
	{
		auto const dampening_factor = 1.0f;
		auto& p_verts = _vertex_field.GetVertices();

		auto height = _vertex_field.GetHeight();
		auto width = _vertex_field.GetWidth();

		for (auto j = 0; j < height; ++j)
		{
			for (auto i = 0; i < width; ++i)
			{
				auto index = j * height + i;

				TerrainVertex& vert = p_verts[index];

				auto distance = Vector3::Distance(location, vert.position);

				if (distance <= radius)
				{
					auto distance_factor = 1 - (distance / radius);

					auto amount = lerp(0.0, intensity, distance_factor);

					vert.position.y += (amount / dampening_factor);
				}
			}
		}
	}

	float GetHighestPoint() const
	{
		return highest_point;
	}

	std::vector<TerrainCell>& GetHeightmapData()
	{
		return _heightmap;
	}

	unsigned int GetWidth() const { return _width; }
	unsigned int GetHeight() const { return _height; }

	void Save()
	{
	}

	void AddImpl(int index, Vector3& normal, int faces)
	{
		auto& cell = _heightmap[index];
		if (faces == 1)
			normal += cell.GetFaceNormal1();
		else if (faces == 2)
			normal += cell.GetFaceNormal2();
		else if (faces == 0)
		{
			normal += cell.GetFaceNormal1();
			normal += cell.GetFaceNormal2();
		}
	}

	void AddDown(int i, int j, Vector3& normal, int faces)
	{
		if (j != 0)
		{
			// can add
			auto index = (j - 1) * _height + i;
			AddImpl(index, normal, faces);
		}
	}

	void AddLeft(int i, int j, Vector3& normal, int faces)
	{
		if (i != 0)
		{
			// can add
			auto index = j * _height + (i - 1);
			AddImpl(index, normal, faces);
		}
	}

	void AddRight(int i, int j, Vector3& normal, int faces)
	{
		if (i < (_width - 1))
		{
			auto index = j * _height + (i + 1);
			AddImpl(index, normal, faces);
		}
	}

	void AddUp(int i, int j, Vector3& normal, int faces)
	{
		if (j < (_height - 1))
		{
			auto index = (j + 1) * _height + (i);
			AddImpl(index, normal, faces);
		}
	}

	void AddBottomLeft(int i, int j, Vector3& normal, int faces)
	{
		if (i != 0 && j != 0)
		{
			// good to go
			auto index = (j - 1) * _height + (i - 1);
			AddImpl(index, normal, faces);
		}
	}

	void AddBottomRight(int i, int j, Vector3& normal, int faces)
	{
		if (j != 0 && i < (_width - 1))
		{
			// good to go
			auto index = (j - 1) * _height + (i + 1);
			AddImpl(index, normal, faces);
		}
	}

	void AddUpperLeft(int i, int j, Vector3& normal, int faces)
	{
		if (i != 0 && j < (_height - 1))
		{
			// good to go
			auto index = (j + 1) * _height + (i - 1);
			AddImpl(index, normal, faces);
		}
	}

	void AddUpperRight(int i, int j, Vector3& normal, int faces)
	{
		if (j < (_height - 1) && i < (_width - 1))
		{
			// good to go
			auto index = (j + 1) * _height + (i + 1);
			AddImpl(index, normal, faces);
		}
	}

	bool CalculateNormalsDifferently(Vector3 position, float radius)
	{
		// Optimization: Only re-calculate normals if the cell is within Position + Radius + Buffer

		for(auto j = 0; j < _height; ++j)
		{
			for(auto i = 0; i < _width; ++i)
			{
				auto index = j * _height + i;

				// each cell has 2 faces, shared between 6 vertices
				auto& cell = _heightmap[index];

				auto distance = Vector3::Distance(position, cell.GetAveragePosition());
				if (distance <= radius)
				{
					// clear out the normals
					cell.ClearNormals();
					cell.SetNormal();
				}
			}
		}

		for (auto j = 0; j < _height; ++j)
		{
			for (auto i = 0; i < _width; ++i)
			{
				auto index = j * _height + i;

				auto& cell = _heightmap[index];

				auto distance = Vector3::Distance(position, cell.GetAveragePosition());

				if (distance <= radius)
				{
					// bottomLeft vertex
					// Need: Face2 on Left, Face1/2 on Bottom-Left, Face1 on Bottom
					cell.data.bottomLeft->normal += cell.GetFaceNormal1();
					cell.data.bottomLeft->normal += cell.GetFaceNormal2();
					AddLeft(i, j, cell.data.bottomLeft->normal, 2);
					AddBottomLeft(i, j, cell.data.bottomLeft->normal, 3);
					AddDown(i, j, cell.data.bottomLeft->normal, 1);
					cell.data.bottomLeft->normal.Normalize();

					// bottomRight vertex
					// Need: Face1 on Bottom, 
					cell.data.bottomRight->normal += cell.GetFaceNormal2();
					AddDown(i, j, cell.data.bottomRight->normal, 3);
					AddBottomRight(i, j, cell.data.bottomRight->normal, 1);
					AddRight(i, j, cell.data.bottomRight->normal, 3);
					cell.data.bottomRight->normal.Normalize();

					// upperLeft vertex
					// Need: Face1/2 Above, Face2 Upper-left, Face1/2 Left, Face1 on self
					cell.data.upperLeft->normal += cell.GetFaceNormal1(); //1
					AddUp(i, j, cell.data.upperLeft->normal, 3); //2
					AddUpperLeft(i, j, cell.data.upperLeft->normal, 2);// 1
					AddLeft(i, j, cell.data.upperLeft->normal, 3); // 2
					cell.data.upperLeft->normal.Normalize();

					// upperRight vertex
					// face1/2 on self, face2 above, face1/2 upper-right, face1 right
					cell.data.upperRight->normal += cell.GetFaceNormal1();
					cell.data.upperRight->normal += cell.GetFaceNormal2();
					AddUp(i, j, cell.data.upperRight->normal, 2);
					AddUpperRight(i, j, cell.data.upperRight->normal, 3);
					AddRight(i, j, cell.data.upperRight->normal, 1);
					cell.data.upperRight->normal.Normalize();

					cell.NormalizeNormals(); // ?? maybe?
				}


			}
		}

		DebugLineManager::GetInstance().Clear();

		// Add debug lines
		for (auto j = 0; j < _height; ++j)
		{
			for (auto i = 0; i < _width; ++i)
			{
				auto& cell = _heightmap[j * _height + i];

				// draw 4 normals lines for debug
				DebugLineManager::GetInstance().AddNormal(cell.data.upperLeft->position, cell.data.upperLeft->normal);
				DebugLineManager::GetInstance().AddNormal(cell.data.upperRight->position, cell.data.upperRight->normal);
				DebugLineManager::GetInstance().AddNormal(cell.data.bottomLeft->position, cell.data.bottomLeft->normal);
				DebugLineManager::GetInstance().AddNormal(cell.data.bottomRight->position, cell.data.bottomRight->normal);
			}
		}

		return true;
	}

	// A utility method which returns the index of the cell which is within a given chunk
	// i and j refer to a selector within the grid
	unsigned int GetIndex(unsigned int chunk_y, unsigned int chunk_x, unsigned int chunk_width, unsigned int chunk_height,
		unsigned int j, unsigned int i)
	{
		/* For this function we assume that the VertexField is partioned like such:
		 
			(chunk_width = 2, chunk_height = 2)
			CCDD
			CCDD
			AABB
			AABB
			
			The section labeled with As would be chunk_y = 0, chunk_x = 0,
			while chunk_y=1, chunk_x=1 would be D.

			From there, i and j, refer to a cell within that chunk.

			Important to remember is there is actually _width + 1 and _height + 1
			number of vertices. This is because A(1) and A(2) share two corners.
		*/

		// We need an index which points to the first cell
		// of a given chunk based on chunk_y, chunk_x, and their dimensions
		unsigned int base_index = (chunk_x * chunk_width) + (chunk_y * (chunk_height)) * (_height + 1);

		// Next, we can use i and j to select the element within our grid
		// remembering that to move up a row, we must consider the entire height
		base_index += i + j * (_height + 1);

		return base_index;
	}

	TerrainVertexField* GetVertexField()
	{
		return &_vertex_field;
	}

private:
	TerrainCell* GetHeightmapDataPointer()
	{
		return &_heightmap[0];
	}

	void CalculateHighestPoint();
	float highest_point;
	unsigned int _width;
	unsigned int _height;
	std::vector<TerrainCell> _heightmap;
	TerrainVertexField _vertex_field;
};
