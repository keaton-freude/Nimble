#pragma once

#include <d3d11.h>
#include "TerrainShader.h"
#include "ShaderManager.h"
#include "Mesh.h"
#include "Transform.h"

using std::shared_ptr;
using Microsoft::WRL::ComPtr;


class TerrainMesh
{
public:
	TerrainMesh() = default;
	TerrainMesh(D3DDevice device, vector<TerrainVertex>& vertices, vector<unsigned long>& indices);
	
	TerrainMesh(const TerrainMesh& other)
	{
		
	}

	~TerrainMesh();

	void Draw(D3DDeviceContext deviceContext, int indexCount);
	void Update(D3DDeviceContext deviceContext, int x, int y, int width, shared_ptr<TerrainVertex> vertices) const;
	Mesh* GetVertexBuffer()
	{
		return mesh.get();
	}

private:
	unique_ptr<Mesh> mesh;
	Transform transform;
	TerrainShader* terrain_shader;
};