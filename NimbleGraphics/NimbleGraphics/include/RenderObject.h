#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "RayHit.h"
#include "WVPShaderComponent.h"
#include "DiffuseShader.h"

class RenderObject
{
public:
	RenderObject() = default;
	RenderObject(shared_ptr<Mesh> mesh, shared_ptr<Material> material, Transform transform);

	void Update(MatrixRef viewMatrix, MatrixRef projectionMatrix);
	void Draw(D3DDevice device, D3DDeviceContext deviceContext);
	RayHit CastRay(const Ray& ray);

private:
	shared_ptr<Mesh> _mesh;
	shared_ptr<Material> _material;
	Transform _transform;
};
