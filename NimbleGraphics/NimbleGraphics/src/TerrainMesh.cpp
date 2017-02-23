//#include "TerrainMesh.h"
//
//TerrainMesh::TerrainMesh(D3DDevice device, vector<TerrainVertex>& vertices, vector<unsigned long>& indices)
//{
//	terrain_shader = ShaderManager::GetInstance().GetShader<TerrainShader>(SHADER::TERRAIN);
//	mesh = Mesh::Create<TerrainVertex>(device, vertices, indices, true);
//}
//
//TerrainMesh::~TerrainMesh()
//{
//	LOG_INFO("TerrainMesh destruct!");
//}
//
//void TerrainMesh::Draw(D3DDeviceContext deviceContext, int indexCount)
//{
//	// bind our vertex buffer
//	unsigned int stride;
//	unsigned int offset;
//
//	// Set vertex buffer stride and offset.
//	stride = sizeof(TerrainVertex);
//	offset = 0;
//
//	// Set the vertex buffer to active in the input assembler so it can be rendered.
//	deviceContext->IASetVertexBuffers(0, 1, mesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
//
//	deviceContext->IASetIndexBuffer(mesh->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
//
//	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
//	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	terrain_shader->SetWorldMatrix(transform.GetWorldMatrix(false));
//	terrain_shader->RenderShader(indexCount);
//}