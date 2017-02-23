#pragma once
#include "Typedefs.h"

#include "Mesh.h"
#include "Transform.h"

class Mesh;

class LineMesh
{
public:
	LineMesh();
	LineMesh(D3DDevice device, Vector3 p1, Vector3 p2)
	{
		
	}

	void Draw(D3DDeviceContext deviceContext, MatrixRef viewMatrix, MatrixRef projectionMatrix);

private:
	unique_ptr<Mesh> mesh;
	Transform transform;
};
