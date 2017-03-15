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
	TileVertexField(unsigned int width, unsigned int height, float resolution);
	~TileVertexField();

	std::vector<TileVertex>& GetVertices();

	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
	TileVertex& GetVert(int i, int j);
	unsigned int GetVertIndex(int i, int j) const;

private:
	std::vector<TileVertex> _vertices;
	unsigned int width;
	unsigned int height;
};