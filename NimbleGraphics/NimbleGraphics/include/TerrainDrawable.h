#pragma once

#include <d3d11.h>
#include "IDrawable.h"
#include "TerrainShader.h"
#include "Light.h"
#include "Texture.h"
#include "ShaderManager.h"

using std::shared_ptr;
using Microsoft::WRL::ComPtr;


class TerrainDrawable : public IDrawable
{
public:
	TerrainDrawable(): terrain_shader(nullptr)
	{
	}

	TerrainDrawable(ComPtr<ID3D11Device> device, vector<TerrainVertex>& vertices, vector<unsigned long>& indices): terrain_shader(nullptr)
	{

		//vertices[0].texture = Vector2(0.0f, 1.0f);
		//vertices[1].texture = Vector2(1.0f, 1.0f);
		//vertices[2].texture = Vector2(2.0f, 1.0f);
		//vertices[3].texture = Vector2(0.0f, 0.0f);
		//vertices[4].texture = Vector2(1.0f, 0.0f);
		//vertices[5].texture = Vector2(2.0f, 0.0f);
		//vertices[6].texture = Vector2(0.0f, -1.0f);
		//vertices[7].texture = Vector2(1.0f, -1.0f);
		//vertices[8].texture = Vector2(2.0f, -1.0f);

		vertexBuffer = VertexBuffer::CreateTerrainNode(device, vertices, indices);
	}

	~TerrainDrawable()
	{
		LOG_INFO("TerrainDrawable destruct!");
	}
	
	void Draw(ComPtr<ID3D11DeviceContext> deviceContext, int indexCount)
	{
		// bind our vertex buffer
		unsigned int stride;
 		unsigned int offset;

		// Set vertex buffer stride and offset.
		stride = sizeof(TerrainVertex);
		offset = 0;

		// Set the vertex buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetVertexBuffers(0, 1, vertexBuffer->GetVertexBuffer().GetAddressOf(), &stride, &offset);

		deviceContext->IASetIndexBuffer(this->vertexBuffer->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// Get pointer to colorShader

		auto terrainShader = ShaderManager::GetInstance().GetShader<TerrainShader>(SHADER::TERRAIN);

		terrainShader->SetWorldMatrix(this->_transform.GetWorldMatrix(false));
		terrainShader->RenderShader(deviceContext, indexCount);
	}

	void Update(ComPtr<ID3D11DeviceContext> deviceContext, int x, int y, int width, shared_ptr<TerrainVertex> vertices)
	{
		D3D11_MAPPED_SUBRESOURCE resource;

		// take our VertexBuffer object, Map it, write our data over the old data, Unmap it
		deviceContext->Map(this->vertexBuffer->GetVertexBuffer().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		TerrainVertex* dataPtr = static_cast<TerrainVertex*>(resource.pData);

		for (int iy = y; iy < width; ++iy)
		{
			for (int ix = x; ix < width; ++ix)
			{
				dataPtr[iy * width + ix] = vertices.get()[iy * width + ix];
			}
		}

		deviceContext->Unmap(this->vertexBuffer->GetVertexBuffer().Get(), 0);
	}
private:
	TerrainShader* terrain_shader;
};