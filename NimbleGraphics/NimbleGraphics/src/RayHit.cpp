#include "RayHit.h"

RayHit::RayHit(): hit(false), distance(0)
{
}

RayHit::RayHit(Vector3 origin, Vector3 hitLocation, float distance, bool hit): hit(hit), distance(distance), origin(origin), hit_location(hitLocation)
{
}

RayHit::~RayHit()
{
}

RayHit RayHit::NoHit()
{
	static auto noHit = RayHit(Vector3::Zero, Vector3::Zero, 0.0f, false);
	return noHit;
}
