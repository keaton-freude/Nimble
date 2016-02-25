#include "RayHit.h"
#include "Logger.h"

const RayHit RayHit::NoHit(Vector3::Zero, Vector3::Zero, 0.0f, false);

RayHit::RayHit()
	: origin(0.0f, 0.0f, 0.0f), hit_location(0.0f, 0.0f, 0.0f),
	distance(0.0f), hit(false)
{

}

RayHit::RayHit(Vector3 origin, Vector3 hitLocation, float distance, bool hit)
	: origin(origin), hit_location(hitLocation), distance(distance), hit(hit)
{
}

RayHit::~RayHit()
{
	//LOG_INFO("RayHit Destruct");
}