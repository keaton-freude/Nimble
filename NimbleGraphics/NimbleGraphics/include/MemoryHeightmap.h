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

	unsigned int GetWidth() const;
	unsigned int GetHeight() const;

	void AddImpl(unsigned int index, Vector3& normal, int faces);
	void AddDown(unsigned int i, unsigned int j, Vector3& normal, int faces);
	void AddLeft(unsigned int i, unsigned int j, Vector3& normal, int faces);
	void AddRight(unsigned int i, unsigned int j, Vector3& normal, int faces);
	void AddUp(unsigned int i, unsigned int j, Vector3& normal, int faces);
	void AddBottomLeft(unsigned int i, unsigned int j, Vector3& normal, int faces);
	void AddBottomRight(unsigned int i, unsigned int j, Vector3& normal, int faces);
	void AddUpperLeft(unsigned int i, unsigned int j, Vector3& normal, int faces);
	void AddUpperRight(unsigned int i, unsigned int j, Vector3& normal, int faces);

	bool CalculateNormalsDifferently(Vector3 position, float radius);

	// A utility method which returns the index of the cell which is within a given chunk
	// i and j refer to a selector within the grid
	unsigned int GetIndex( unsigned int chunk_y, unsigned int chunk_x, unsigned int chunk_width,
						unsigned int chunk_height, unsigned int j, unsigned int i);

	TileVertexField* GetVertexField();
	RayHit IsRayIntersectingVerts(const Ray& ray);

private:
	void CalculateHighestPoint();
	float highest_point;
	unsigned int _width;
	unsigned int _height;
	std::vector<TileCell> _heightmap;
	TileVertexField _vertex_field;
};