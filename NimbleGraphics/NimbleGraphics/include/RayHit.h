#pragma once
#include "Typedefs.h"

class RayHit
{
public:
	RayHit();

	RayHit(Vector3 origin, Vector3 hitLocation, float distance, bool hit);

	~RayHit();

	static RayHit NoHit();

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