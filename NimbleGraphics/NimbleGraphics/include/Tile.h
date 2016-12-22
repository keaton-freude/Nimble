#pragma once
#include "MemoryHeightmap.h"
#include "D3D11Object.h"

class Tile : public D3D11Object
{
public: 
	Tile();
	~Tile();
	void Draw();
private:
	// defines the topographical features of the map
	MemoryHeightmap _heightmap;
};
