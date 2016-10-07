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
	TerrainVertexField();
	TerrainVertexField(unsigned int width, unsigned int height, float resolution);
	~TerrainVertexField();

	std::vector<TerrainVertex>& GetVertices();

	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
	TerrainVertex& GetVert(int i, int j);
	unsigned int GetVertIndex(int i, int j) const;

private:
	std::vector<TerrainVertex> _vertices;
	unsigned int width;
	unsigned int height;
};