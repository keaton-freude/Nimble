#include "MemoryHeightmap.h"
#include "Helper.h"

MemoryHeightmap::MemoryHeightmap()
	: highest_point(0.0f), _width(0), _height(0), _heightmap(), _vertex_field()
{
}

MemoryHeightmap::~MemoryHeightmap()
{
}

MemoryHeightmap::MemoryHeightmap(unsigned int width, unsigned int height, float resolution)
	: highest_point(0.0f), _width(width), _height(height), _vertex_field(width + 1, height + 1, resolution)
{
	_heightmap.reserve(width * height);

	auto& p_verts = _vertex_field.GetVertices();
	for (unsigned int j = 0; j < _height; ++j)
	{
		for (unsigned int i = 0; i < _width; ++i)
		{
			auto vf_height = _vertex_field.GetHeight();
			auto index = j * _height + i;

			auto blIndex = j * vf_height + i;
			auto brIndex = j * vf_height + (i + 1);
			auto ulIndex = (j + 1) * vf_height + i;
			auto urIndex = (j + 1) * vf_height + (i + 1);

			// Create _symbolic_ links to the underlying verts.
			_heightmap.push_back(TileCell(p_verts[ulIndex], p_verts[urIndex], p_verts[blIndex], p_verts[brIndex]));
		}
	}

	CalculateTextureCoordinates();
}

void MemoryHeightmap::CalculateTextureCoordinates()
{
	float incrementValue = static_cast<float>(TEXTURE_REPEAT) / _width;

	float tu, tv;

	tv = 1.0f * TEXTURE_REPEAT;
	tu = 0.0f;

	auto vf_height = _vertex_field.GetHeight();
	auto vf_width = _vertex_field.GetWidth();

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

void MemoryHeightmap::SmoothAdd(const Vector3& location, const float& radius, const float& intensity)
{
	auto const dampening_factor = 1.0f;
	auto& p_verts = _vertex_field.GetVertices();

	auto height = _vertex_field.GetHeight();
	auto width = _vertex_field.GetWidth();

	for (unsigned int j = 0; j < height; ++j)
	{
		for (unsigned int i = 0; i < width; ++i)
		{
			auto index = j * height + i;

			TileVertex& vert = p_verts[index];

			auto distance = Vector2XZDistance(location, vert.position);

			if (distance <= radius)
			{
				auto distance_factor = 1 - (distance / radius);

				auto amount = lerp(0.0, intensity, distance_factor);

				vert.position.y += (amount / dampening_factor);
			}
		}
	}
}

float MemoryHeightmap::GetHighestPoint() const
{
	return highest_point;
}

std::vector<TileCell>& MemoryHeightmap::GetHeightmapData()
{
	return _heightmap;
}

Dimension MemoryHeightmap::GetWidth() const
{
	return _width;
}

Dimension MemoryHeightmap::GetHeight() const
{
	return _height;
}

void MemoryHeightmap::AddImpl(Dimension index, Vector3& normal, int faces)
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

void MemoryHeightmap::AddDown(Dimension i, Dimension j, Vector3& normal, int faces)
{
	if (j != 0)
	{
		// can add
		auto index = (j - 1) * _height + i;
		AddImpl(index, normal, faces);
	}
}

void MemoryHeightmap::AddLeft(Dimension i, Dimension j, Vector3& normal, int faces)
{
	if (i != 0)
	{
		// can add
		auto index = j * _height + (i - 1);
		AddImpl(index, normal, faces);
	}
}

void MemoryHeightmap::AddRight(Dimension i, Dimension j, Vector3& normal, int faces)
{
	if (i < (_width - 1))
	{
		auto index = j * _height + (i + 1);
		AddImpl(index, normal, faces);
	}
}

void MemoryHeightmap::AddUp(Dimension i, Dimension j, Vector3& normal, int faces)
{
	if (j < (_height - 1))
	{
		auto index = (j + 1) * _height + (i);
		AddImpl(index, normal, faces);
	}
}

void MemoryHeightmap::AddBottomLeft(Dimension i, Dimension j, Vector3& normal, int faces)
{
	if (i != 0 && j != 0)
	{
		// good to go
		auto index = (j - 1) * _height + (i - 1);
		AddImpl(index, normal, faces);
	}
}

void MemoryHeightmap::AddBottomRight(Dimension i, Dimension j, Vector3& normal, int faces)
{
	if (j != 0 && i < (_width - 1))
	{
		// good to go
		auto index = (j - 1) * _height + (i + 1);
		AddImpl(index, normal, faces);
	}
}

void MemoryHeightmap::AddUpperLeft(Dimension i, Dimension j, Vector3& normal, int faces)
{
	if (i != 0 && j < (_height - 1))
	{
		// good to go
		auto index = (j + 1) * _height + (i - 1);
		AddImpl(index, normal, faces);
	}
}

void MemoryHeightmap::AddUpperRight(Dimension i, Dimension j, Vector3& normal, int faces)
{
	if (j < (_height - 1) && i < (_width - 1))
	{
		// good to go
		auto index = (j + 1) * _height + (i + 1);
		AddImpl(index, normal, faces);
	}
}

bool MemoryHeightmap::CalculateNormalsDifferently(Vector3 position, float radius)
{
	// Optimization: Only re-calculate normals if the cell is within Position + Radius + Buffer

	for (auto j = 0; j < _height; ++j)
	{
		for (auto i = 0; i < _width; ++i)
		{
			auto index = j * _height + i;

			// each cell has 2 faces, shared between 6 vertices
			auto& cell = _heightmap[index];

			auto distance = Vector3::Distance(position, cell.GetAveragePosition());
			if (distance <= radius)
			{
				// Recalculate the normals of each cell that is affected
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
				cell.data.bottomLeft.normal += cell.GetFaceNormal1();
				cell.data.bottomLeft.normal += cell.GetFaceNormal2();
				AddLeft(i, j, cell.data.bottomLeft.normal, 2);
				AddBottomLeft(i, j, cell.data.bottomLeft.normal, 3);
				AddDown(i, j, cell.data.bottomLeft.normal, 1);
				cell.data.bottomLeft.normal.Normalize();

				// bottomRight vertex
				// Need: Face1 on Bottom, 
				cell.data.bottomRight.normal += cell.GetFaceNormal2();
				AddDown(i, j, cell.data.bottomRight.normal, 3);
				AddBottomRight(i, j, cell.data.bottomRight.normal, 1);
				AddRight(i, j, cell.data.bottomRight.normal, 3);
				cell.data.bottomRight.normal.Normalize();

				// upperLeft vertex
				// Need: Face1/2 Above, Face2 Upper-left, Face1/2 Left, Face1 on self
				cell.data.upperLeft.normal += cell.GetFaceNormal1(); //1
				AddUp(i, j, cell.data.upperLeft.normal, 3); //2
				AddUpperLeft(i, j, cell.data.upperLeft.normal, 2);// 1
				AddLeft(i, j, cell.data.upperLeft.normal, 3); // 2
				cell.data.upperLeft.normal.Normalize();

				// upperRight vertex
				// face1/2 on self, face2 above, face1/2 upper-right, face1 right
				cell.data.upperRight.normal += cell.GetFaceNormal1();
				cell.data.upperRight.normal += cell.GetFaceNormal2();
				AddUp(i, j, cell.data.upperRight.normal, 2);
				AddUpperRight(i, j, cell.data.upperRight.normal, 3);
				AddRight(i, j, cell.data.upperRight.normal, 1);
				cell.data.upperRight.normal.Normalize();

				//cell.NormalizeNormals(); // ?? maybe?
			}
		}
	}

	return true;
}

Dimension MemoryHeightmap::GetIndex(unsigned int chunk_y, unsigned int chunk_x, unsigned int chunk_width, unsigned int chunk_height, unsigned int j, unsigned int i)
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
	Dimension base_index = (chunk_x * chunk_width) + (chunk_y * (chunk_height)) * (_height + 1);

	// Next, we can use i and j to select the element within our grid
	// remembering that to move up a row, we must consider the entire height
	base_index += i + j * (_height + 1);

	return base_index;
}

TileVertexField* MemoryHeightmap::GetVertexField()
{
	return &_vertex_field;
}

RayHit MemoryHeightmap::IsRayIntersectingVerts(const Ray& ray)
{	
	for (int i = 0; i < _height * _width; ++i)
	{
	    // every cell has 2 triangles
	    auto& current_cell = _heightmap[i];
	
	    Vector3 triangle1[3];
	    Vector3 triangle2[3];
	
	    triangle1[0] = current_cell.data.upperLeft.position;
	    triangle1[1] = current_cell.data.upperRight.position;
	    triangle1[2] = current_cell.data.bottomLeft.position;
	
	    triangle2[0] = current_cell.data.bottomLeft.position;
	    triangle2[1] = current_cell.data.upperRight.position;
	    triangle2[2] = current_cell.data.bottomRight.position;
	
	    float distance;
	
	    if (rayTriangleIntersect(ray, triangle1, distance))
	    {
	        // log_info("ray triangle intersect. distance: ", distance);
	
	        Vector3 hit_location = (ray.position + (ray.direction * distance));
	        return RayHit(ray.position, hit_location, distance, true);
	    }
	
	    if (rayTriangleIntersect(ray, triangle2, distance))
	    {
	        // og_info("ray triangle intersect. distance: ", distance);
	
	        Vector3 hit_location = (ray.position + (ray.direction * distance));
	        return RayHit(ray.position, hit_location, distance, true);
	    }
	}
	
	return RayHit::NoHit();
}

void MemoryHeightmap::CalculateHighestPoint()
{
}
