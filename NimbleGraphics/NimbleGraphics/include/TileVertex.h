#pragma once
#include <fstream>
#include "Structs.h"

struct TileVertex: public IVertex
{
	Vector3 position;
	Vector2 texture;
	Vector3 normal;

	TileVertex()
		: position(Vector3::Zero), texture(Vector2::Zero), normal(Vector3(0.00f, 1.0f, 0.00f))
	{
	}

	TileVertex(Vector3 pos, Vector2 tex, Vector3 norm)
		: position(pos), texture(tex), normal(norm)
	{

	}

	void WriteToFile(std::fstream & file) const
	{
		file << position.x << "," << position.y << "," << position.z << "," << texture.x << "," << texture.y << ","
			<< normal.x << "," << normal.y << "," << normal.z;
	}

};
