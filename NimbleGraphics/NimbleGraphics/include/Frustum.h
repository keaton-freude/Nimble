#pragma once
#include <SimpleMath.h>

using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Plane;

class Frustum
{
public:
	Frustum();
	Frustum(const Frustum&);
	~Frustum();

	void ConstructFrustum(float, Matrix, Matrix);

	bool CheckPoint(float, float, float);
	bool CheckCube(float, float, float, float);
	bool CheckSphere(float, float, float, float);
	bool CheckRectangle(float, float, float, float, float, float);

private:
	Plane _planes[6];
};