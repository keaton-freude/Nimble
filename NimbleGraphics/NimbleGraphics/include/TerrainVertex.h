#pragma once
#include "Typedefs.h"
#include <fstream>

struct TerrainVertex
{
	Vector3 position;
	Vector2 texture;
	Vector3 normal;

	TerrainVertex()
		: position(Vector3::Zero), texture(Vector2::Zero), normal(Vector3(0.00f, 1.0f, 0.00f))
	{
	}

	TerrainVertex(Vector3 pos, Vector2 tex, Vector3 norm)
		: position(pos), texture(tex), normal(norm)
	{

	}

	void WriteToFile(std::fstream & file) const
	{
		file << position.x << "," << position.y << "," << position.z << "," << texture.x << "," << texture.y << ","
			<< normal.x << "," << normal.y << "," << normal.z;
	}

};
