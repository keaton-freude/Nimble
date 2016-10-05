#pragma once
#include "Typedefs.h"

#include "Mesh.h"
#include "Transform.h"

class LineMesh
{
public:
	LineMesh();
	LineMesh(D3DDevice device, Vector3 p1, Vector3 p2);
	LineMesh(LineMesh&& other);
	LineMesh(const LineMesh& other)
	{
		
	}
	~LineMesh();

	void Draw(D3DDeviceContext deviceContext, MatrixRef viewMatrix, MatrixRef projectionMatrix);

private:
	unique_ptr<Mesh> mesh;
	Transform transform;
};
