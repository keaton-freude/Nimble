#include "Frustum.h"
#include "Logger.h"

Frustum::Frustum()
{
}

Frustum::Frustum(const Frustum&)
{
}

Frustum::~Frustum()
{
	LOG_INFO("Frustum destruct.");
}

void Frustum::ConstructFrustum(float screenDepth, Matrix projectionMatrix, Matrix viewMatrix)
{
	float zMinimum, r;
	Matrix matrix;


	// Calculate the minimum Z distance in the frustum.
	zMinimum = -projectionMatrix._43 / projectionMatrix._33;
	r = screenDepth / (screenDepth - zMinimum);
	projectionMatrix._33 = r;
	projectionMatrix._43 = -r * zMinimum;

	// Create the frustum matrix from the view matrix and updated projection matrix.
	matrix = viewMatrix * projectionMatrix;

	// Calculate near plane of frustum.
	_planes[0].x = matrix._14 + matrix._13;
	_planes[0].y = matrix._24 + matrix._23;
	_planes[0].z = matrix._34 + matrix._33;
	_planes[0].w = matrix._44 + matrix._43;
	_planes[0].Normalize();

	// Calculate far plane of frustum.
	_planes[1].x = matrix._14 - matrix._13;
	_planes[1].y = matrix._24 - matrix._23;
	_planes[1].z = matrix._34 - matrix._33;
	_planes[1].w = matrix._44 - matrix._43;
	_planes[1].Normalize();

	// Calculate left plane of frustum.
	_planes[2].x = matrix._14 + matrix._11;
	_planes[2].y = matrix._24 + matrix._21;
	_planes[2].z = matrix._34 + matrix._31;
	_planes[2].w = matrix._44 + matrix._41;
	_planes[2].Normalize();

	// Calculate right plane of frustum.
	_planes[3].x = matrix._14 - matrix._11;
	_planes[3].y = matrix._24 - matrix._21;
	_planes[3].z = matrix._34 - matrix._31;
	_planes[3].w = matrix._44 - matrix._41;
	_planes[3].Normalize();

	// Calculate top plane of frustum.
	_planes[4].x = matrix._14 - matrix._12;
	_planes[4].y = matrix._24 - matrix._22;
	_planes[4].z = matrix._34 - matrix._32;
	_planes[4].w = matrix._44 - matrix._42;
	_planes[4].Normalize();

	// Calculate bottom plane of frustum.
	_planes[5].x = matrix._14 + matrix._12;
	_planes[5].y = matrix._24 + matrix._22;
	_planes[5].z = matrix._34 + matrix._32;
	_planes[5].w = matrix._44 + matrix._42;
	_planes[5].Normalize();
}

bool Frustum::CheckPoint(float x, float y, float z)
{
	int i;

	// Check if the point is inside all six planes of the view frustum.
	for (i = 0; i < 6; i++)
	{
		if (_planes[i].DotCoordinate(DirectX::SimpleMath::Vector3(x, y, z)) < 0.0f)
		{
			return false;
		}
	}

	return true;
}

bool Frustum::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
{
	int i;


	// Check if any one point of the cube is in the view frustum.
	for (i = 0; i < 6; i++)
	{
		if (_planes[i].DotCoordinate(DirectX::SimpleMath::Vector3(xCenter - radius, yCenter - radius, zCenter - radius)) >= 0.0f)
		{
			continue;
		}

		if (_planes[i].DotCoordinate(DirectX::SimpleMath::Vector3(xCenter + radius, yCenter - radius, zCenter - radius)) >= 0.0f)
		{
			continue;
		}

		if (_planes[i].DotCoordinate(Vector3(xCenter - radius, yCenter + radius, zCenter - radius)) >= 0.0f)
		{
			continue;
		}

		if (_planes[i].DotCoordinate(Vector3(xCenter + radius, yCenter + radius, zCenter - radius)) >= 0.0f)
		{
			continue;
		}

		if (_planes[i].DotCoordinate(Vector3(xCenter - radius, yCenter - radius, zCenter + radius)) >= 0.0f)
		{
			continue;
		}

		if (_planes[i].DotCoordinate(Vector3(xCenter + radius, yCenter - radius, zCenter + radius)) >= 0.0f)
		{
			continue;
		}

		if (_planes[i].DotCoordinate(Vector3(xCenter - radius, yCenter + radius, zCenter + radius)) >= 0.0f)
		{
			continue;
		}

		if (_planes[i].DotCoordinate(Vector3(xCenter + radius, yCenter + radius, zCenter + radius)) >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}

bool Frustum::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
{
	int i;


	// Check if the radius of the sphere is inside the view frustum.
	for (i = 0; i < 6; i++)
	{
		if (_planes[i].DotCoordinate(Vector3(xCenter, yCenter, zCenter)) < -radius)
		{
			return false;
		}
	}

	return true;
}

bool Frustum::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
{
	int i;


	// Check if any of the 6 planes of the rectangle are inside the view frustum.
	for (i = 0; i < 6; i++)
	{
		if (_planes[i].DotCoordinate(Vector3(xCenter - xSize, yCenter - ySize, zCenter - zSize)) >= 0.0f)
		{
			continue;
		}

		if (_planes[i].DotCoordinate(Vector3(xCenter + xSize, yCenter - ySize, zCenter - zSize)) >= 0.0f)
		{
			continue;
		}

		if (_planes[i].DotCoordinate(Vector3(xCenter - xSize, yCenter + ySize, zCenter - zSize)) >= 0.0f)
		{
			continue;
		}

		if (_planes[i].DotCoordinate(Vector3(xCenter - xSize, yCenter - ySize, zCenter + zSize)) >= 0.0f)
		{
			continue;
		}

		if (_planes[i].DotCoordinate(Vector3(xCenter + xSize, yCenter + ySize, zCenter - zSize)) >= 0.0f)
		{
			continue;
		}

		if (_planes[i].DotCoordinate(Vector3(xCenter + xSize, yCenter - ySize, zCenter + zSize)) >= 0.0f)
		{
			continue;
		}

		if (_planes[i].DotCoordinate(Vector3(xCenter - xSize, yCenter + ySize, zCenter + zSize)) >= 0.0f)
		{
			continue;
		}

		if (_planes[i].DotCoordinate(Vector3(xCenter + xSize, yCenter + ySize, zCenter + zSize)) >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}
