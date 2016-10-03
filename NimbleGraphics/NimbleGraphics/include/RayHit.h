#pragma once
#include <SimpleMath.h>

using DirectX::SimpleMath::Vector3;


class RayHit
{
public:
	RayHit(): hit(false), distance(0)
	{
	}

	RayHit(Vector3 origin, Vector3 hitLocation, float distance, bool hit): hit(hit), distance(distance), origin(origin), hit_location(hitLocation)
	{
	}

	~RayHit()
	{
		
	}

	static RayHit NoHit()
	{
		static auto noHit = RayHit(Vector3::Zero, Vector3::Zero, 0.0f, false);
		return noHit;
	}

	// true if the ray intersection test was successful, else false
	// if false, then state of this object is not valid
	bool hit;

	// distance from origin of ray to intersection
	float distance;

	// Location in world space of the ray intersection
	Vector3 hit_location;

	// Location in world space of the origin of the ray
	// NOTE: Maybe un-needed, might be nice to track it all together though
	// and you could pass this RayHit to other modules that maybe didn't have
	// the origin Vector when it was created
	Vector3 origin;
};