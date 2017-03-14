#pragma once
#include <vector>
#include "Typedefs.h"
#include "TileVertex.h"

/* This class represents the actual vertices for a tile.
 * The tile class and other things based on this, create views
 * of this underlying vertex field.
 * 
 * Other classes or tools will access this directly. When adding
 * to the heightmap via a tool, we will modify the underlying
 * Vertex field. Tiles just index the vertices in a 
 * way that the GPU understands.
 */

class TileVertexField
{
public:
	TileVertexField();
	TileVertexField(Dimension width, Dimension height, float resolution);
	~TileVertexField();

	std::vector<TileVertex>& GetVertices();

	Dimension GetWidth() const;
	Dimension GetHeight() const;
	TileVertex& GetVert(int i, int j);
	Dimension GetVertIndex(int i, int j) const;

private:
	std::vector<TileVertex> _vertices;
	Dimension width;
	Dimension height;
};