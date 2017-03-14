#pragma once
#include <vector>
#include "DebugLineManager.h"
#include "TileVertexField.h"
#include "TileCell.h"
#include "RayHit.h"

namespace DirectX {
	namespace SimpleMath {
		struct Vector3;
	}
}

class MemoryHeightmap
{
public:
	MemoryHeightmap();
	~MemoryHeightmap();

	MemoryHeightmap(unsigned int width, unsigned int height, float resolution = 1.0f);
	void CalculateTextureCoordinates();
	void SmoothAdd(const Vector3& location, const float& radius, const float& intensity);
	float GetHighestPoint() const;
	std::vector<TileCell>& GetHeightmapData();

	Dimension GetWidth() const;
	Dimension GetHeight() const;

	void AddImpl(Dimension index, Vector3& normal, int faces);
	void AddDown(Dimension i, Dimension j, Vector3& normal, int faces);
	void AddLeft(Dimension i, Dimension j, Vector3& normal, int faces);
	void AddRight(Dimension i, Dimension j, Vector3& normal, int faces);
	void AddUp(Dimension i, Dimension j, Vector3& normal, int faces);
	void AddBottomLeft(Dimension i, Dimension j, Vector3& normal, int faces);
	void AddBottomRight(Dimension i, Dimension j, Vector3& normal, int faces);
	void AddUpperLeft(Dimension i, Dimension j, Vector3& normal, int faces);
	void AddUpperRight(Dimension i, Dimension j, Vector3& normal, int faces);

	bool CalculateNormalsDifferently(Vector3 position, float radius);

	// A utility method which returns the index of the cell which is within a given chunk
	// i and j refer to a selector within the grid
	Dimension GetIndex( unsigned int chunk_y, unsigned int chunk_x, unsigned int chunk_width,
						unsigned int chunk_height, unsigned int j, unsigned int i);

	TileVertexField* GetVertexField();
	RayHit IsRayIntersectingVerts(const Ray& ray);

private:
	void CalculateHighestPoint();
	float highest_point;
	Dimension _width;
	Dimension _height;
	std::vector<TileCell> _heightmap;
	TileVertexField _vertex_field;
};