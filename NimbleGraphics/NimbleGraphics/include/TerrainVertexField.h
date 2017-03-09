#pragma once
#include <vector>
#include "TerrainCell.h"

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
	TerrainVertexField(Dimension width, Dimension height, float resolution);
	~TerrainVertexField();

	std::vector<TerrainVertex>& GetVertices();

	Dimension GetWidth() const;
	Dimension GetHeight() const;
	TerrainVertex& GetVert(int i, int j);
	Dimension GetVertIndex(int i, int j) const;

private:
	std::vector<TerrainVertex> _vertices;
	Dimension width;
	Dimension height;
};