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
	MemoryHeightmap();

	~MemoryHeightmap();

	MemoryHeightmap(unsigned int width, unsigned int height, float resolution = 1.0f);

	void CalculateTextureCoordinates();

	void SmoothAdd(const Vector3& location, const float& radius, const float& intensity);

	float GetHighestPoint() const;

	std::vector<TerrainCell>& GetHeightmapData();

	unsigned int GetWidth() const;
	unsigned int GetHeight() const;

	void AddImpl(int index, Vector3& normal, int faces);
	void AddDown(int i, int j, Vector3& normal, int faces);
	void AddLeft(int i, int j, Vector3& normal, int faces);
	void AddRight(int i, int j, Vector3& normal, int faces);
	void AddUp(int i, int j, Vector3& normal, int faces);
	void AddBottomLeft(int i, int j, Vector3& normal, int faces);
	void AddBottomRight(int i, int j, Vector3& normal, int faces);
	void AddUpperLeft(int i, int j, Vector3& normal, int faces);
	void AddUpperRight(int i, int j, Vector3& normal, int faces);
	

	bool CalculateNormalsDifferently(Vector3 position, float radius);

	// A utility method which returns the index of the cell which is within a given chunk
	// i and j refer to a selector within the grid
	unsigned int GetIndex(unsigned int chunk_y, unsigned int chunk_x, unsigned int chunk_width, unsigned int chunk_height,
	                      unsigned int j, unsigned int i);



	TerrainVertexField* GetVertexField();

private:
	void CalculateHighestPoint();
	float highest_point;
	unsigned int _width;
	unsigned int _height;
	std::vector<TerrainCell> _heightmap;
	TerrainVertexField _vertex_field;
};
