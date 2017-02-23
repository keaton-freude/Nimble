#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"

class RenderObject
{
public:
	RenderObject();
	RenderObject(shared_ptr<Mesh> mesh, shared_ptr<Material> material, Transform transform);

	void Update(MatrixRef viewMatrix, MatrixRef projectionMatrix);
	void Draw(D3DDevice device, D3DDeviceContext deviceContext);
private:
	shared_ptr<Mesh> _mesh;
	shared_ptr<Material> _material;
	Transform _transform;
};
