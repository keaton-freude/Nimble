#include "RenderObject.h"

RenderObject::RenderObject(shared_ptr<Mesh> mesh, shared_ptr<Material> material, Transform transform)
	: _mesh(mesh), _material(material), _transform(transform)
{

}

void RenderObject::Update(MatrixRef viewMatrix, MatrixRef projectionMatrix)
{
	auto baseShader = _material->GetShader();

	auto componentPtr = static_cast<WVPShaderComponent*>(baseShader->GetComponents()[0].get());
	componentPtr->SetMatrices(_transform.GetWorldMatrix(), viewMatrix, projectionMatrix);
}

void RenderObject::Draw(D3DDevice device, D3DDeviceContext deviceContext)
{
	auto vb = _mesh->GetVertexBuffer().Get();
	unsigned int offset = 0;
	auto stride = _mesh->GetMeshData().stride;

	deviceContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
	deviceContext->IASetIndexBuffer(_mesh->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(_mesh->GetMeshData().topology);

	_material->Apply(deviceContext);
	_material->GetShader()->SetShaderParameters();
	_material->GetShader()->Draw(_mesh->GetMeshData().numIndicies);
}

RayHit RenderObject::CastRay(const Ray& ray)
{
	auto& triangles = _mesh->GetTriangles();

	for (const auto& triangle : triangles)
	{
		float distance;

		if (rayTriangleIntersect(ray, triangle.vertices, distance))
		{
			LOG_INFO("Hit! Distance: ", distance);
			Vector3 hit_location = (ray.position + (ray.direction * distance));
			return RayHit(ray.position, hit_location, distance, true);
		}
	}

	return RayHit::NoHit();
}