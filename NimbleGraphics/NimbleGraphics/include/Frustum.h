#pragma once
#include "Typedefs.h"

class Frustum
{
public:
	Frustum();

	Frustum(const Frustum&);

	~Frustum();

	void ConstructFrustum(float screenDepth, Matrix projectionMatrix, Matrix viewMatrix);

	bool CheckPoint(float x, float y, float z);

	bool CheckCube(float xCenter, float yCenter, float zCenter, float radius);

	bool CheckSphere(float xCenter, float yCenter, float zCenter, float radius);

	bool CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize);

private:
	Plane _planes[6];
};
