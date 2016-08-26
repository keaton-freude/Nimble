#pragma once
#include "Helper.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "Structs.h"

namespace DirectX{
	namespace SimpleMath{
		struct Vector3;
	}
}

class MemoryHeightmap
{
public:
	MemoryHeightmap()
		: highest_point(0.0f), _heightmap(), _width(0), _height(0)
	{
		
	}

	~MemoryHeightmap()
	{
		
	}

	MemoryHeightmap(unsigned int width, unsigned int height, float resolution = 1.0f)
		: highest_point(0.0f), _width(width), _height(height), _heightmap(width * height)
	{
		for(unsigned int j = 0; j < _height; ++j)
		{
			for(unsigned int i = 0; i < _width; ++i)
			{
				auto index = j * _height + i;
				TerrainCell cell = TerrainCell();
				cell.upperLeft = TerrainVertex(Vector3(i, 0.0f, j + 1), Vector2::Zero, Vector3::UnitY);
				cell.upperRight = TerrainVertex(Vector3(i + 1, 0.0f, j + 1), Vector2::Zero, Vector3::UnitY);
				cell.bottomLeft = TerrainVertex(Vector3(i, 0.0f, j), Vector2::Zero, Vector3::UnitY);
				cell.bottomRight = TerrainVertex(Vector3(i + 1, 0.0f, j), Vector2::Zero, Vector3::UnitY);
				
				_heightmap[index] = cell;
			}
		}

		CalculateTextureCoordinates();
	}

	void CalculateTextureCoordinates()
	{
		int incrementCount, tuCount, tvCount;
		float incrementValue, tuCoordinate, tvCoordinate;

		incrementValue = (float)TEXTURE_REPEAT / (float)_width;

		float tu, tv;

		tv = 1.0f;
		tu = 0.0f;

		tuCount = 0;
		tvCount = 0;

		incrementCount = _width / TEXTURE_REPEAT;

		for (unsigned int j = 0; j < _height; ++j)
		{
			for (unsigned int i = 0; i < _width; ++i)
			{
				auto index = j * _height + i;

				auto & current_cell = _heightmap[index];
				
				Vector2 upperLeft = Vector2(tu, tv - incrementValue);
				Vector2 upperRight = Vector2(tu + incrementValue, tv - incrementValue);
				Vector2 bottomLeft = Vector2(tu, tv);
				Vector2 bottomRight = Vector2(tu + incrementValue, tv);

				current_cell.upperLeft.texture = upperLeft;
				current_cell.upperRight.texture = upperRight;
				current_cell.bottomLeft.texture = bottomLeft;
				current_cell.bottomRight.texture = bottomRight;

				// keep tu moving
				tu += incrementValue;
				tuCount++;

				if (tuCount == incrementCount)
				{
					tu = 0.0f;
					tuCount = 0;
				}
			}

			tv -= incrementValue;
			tvCount++;

			if (tvCount == incrementCount)
			{
				tv = 1.0f;
				tvCount = 0;
			}
		}
	}

	void SmoothAdd(DirectX::SimpleMath::Vector3 location, float radius, float intensity)
	{
		auto const dampening_factor = 1.0f;

		for (auto j = 0; j < _height; ++j)
		{
			for (auto i = 0; i < _height; ++i)
			{
				auto index = j * _height + i;

				TerrainCell& currentMapPosition = _heightmap[index];

				// BOTTOM LEFT
				auto distance = Vector3::Distance(location, currentMapPosition.bottomLeft.position);
				if (distance <= radius)
				{
					auto distance_factor = 1 - (distance / radius);

					auto amount = lerp(0.0, intensity, distance_factor);

					currentMapPosition.bottomLeft.position.y += (amount / dampening_factor);
				}
				distance = Vector3::Distance(location, currentMapPosition.bottomRight.position);
				if (distance <= radius)
				{
					auto distance_factor = 1 - (distance / radius);

					auto amount = lerp(0.0, intensity, distance_factor);

					currentMapPosition.bottomRight.position.y += (amount / dampening_factor);
				}
				distance = Vector3::Distance(location, currentMapPosition.upperLeft.position);
				if (distance <= radius)
				{
					auto distance_factor = 1 - (distance / radius);

					auto amount = lerp(0.0, intensity, distance_factor);

					currentMapPosition.upperLeft.position.y += (amount / dampening_factor);
				}
				distance = Vector3::Distance(location, currentMapPosition.upperRight.position);
				if (distance <= radius)
				{
					auto distance_factor = 1 - (distance / radius);

					auto amount = lerp(0.0, intensity, distance_factor);

					currentMapPosition.upperRight.position.y += (amount / dampening_factor);
				}
			}
		}
	}

	float GetHighestPoint() const
	{
		return highest_point;
	}

	std::vector<TerrainCell>& GetHeightmapData()
	{
		return _heightmap;
	}

	unsigned int GetWidth() const { return _width; }
	unsigned int GetHeight() const { return _height; }

	void Save()
	{
		// save current state of _heightmap to terrain_data.txt
		std::fstream file;

		file.open("terrain-data.txt", std::fstream::out);

		for(auto i = 0; i < (_width * _height) - 1; ++i)
		{
			auto& current_cell = _heightmap[i];
			// write these cells data
			current_cell.bottomLeft.WriteToFile(file);
			file << "\n";
			current_cell.bottomRight.WriteToFile(file);
			file << "\n";
			current_cell.upperLeft.WriteToFile(file);
			file << "\n";
			current_cell.upperRight.WriteToFile(file);
			file << "\n";
		}

		auto& current_cell = _heightmap.back();
		// write these cells data
		current_cell.bottomLeft.WriteToFile(file);
		file << "\n";
		current_cell.bottomRight.WriteToFile(file);
		file << "\n";
		current_cell.upperLeft.WriteToFile(file);
		file << "\n";
		current_cell.upperRight.WriteToFile(file);

		file.close();
	}

private:
	bool CalculateNormals();
	void CalculateHighestPoint();
	float highest_point;
	unsigned int _width;
	unsigned int _height;
	std::vector<TerrainCell> _heightmap;

};
