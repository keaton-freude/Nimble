#include "MemoryHeightmap.h"


void MemoryHeightmap::SmoothAdd(DirectX::SimpleMath::Vector3 location, float radius, float intensity)
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

bool MemoryHeightmap::CalculateNormals()
{


	return true;
}

void MemoryHeightmap::CalculateHighestPoint()
{
}
