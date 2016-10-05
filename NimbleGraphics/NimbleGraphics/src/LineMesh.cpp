#include "LineMesh.h"
#include "ShaderManager.h"

LineMesh::LineMesh()
{
}

LineMesh::LineMesh(D3DDevice device, Vector3 p1, Vector3 p2)
{
	mesh = Mesh::CreateLine(device, p1, p2);
}

LineMesh::LineMesh(LineMesh&& other)
{
	this->mesh = std::move(other.mesh);
	this->transform = transform;
}

LineMesh::~LineMesh()
{
	//LOG_INFO("LineMesh destruct!");
}

void LineMesh::Draw(D3DDeviceContext deviceContext, MatrixRef viewMatrix, MatrixRef projectionMatrix)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(ColorVertex);
	offset = 0;
	auto vb = mesh->GetVertexBuffer().Get();
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);

	deviceContext->IASetIndexBuffer(mesh->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// Get pointer to colorShader
	auto colorShader = ShaderManager::GetInstance().GetShader<ColorShader>(SHADER::COLOR);

	colorShader->Draw(deviceContext, mesh->GetNumIndices(), transform.GetWorldMatrix(), viewMatrix, projectionMatrix);
}
