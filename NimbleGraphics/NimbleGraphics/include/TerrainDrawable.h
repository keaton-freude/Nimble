#pragma once

#include <d3d11.h>
#include "IDrawable.h"
#include "TerrainShader.h"
#include "Light.h"
#include "Texture.h"

using std::shared_ptr;
using Microsoft::WRL::ComPtr;


class TerrainDrawable : public IDrawable
{
public:
	TerrainDrawable();
	TerrainDrawable(ComPtr<ID3D11Device> device, shared_ptr<TerrainVertex> vertices, int vertexCount, shared_ptr<unsigned long> indices);

	~TerrainDrawable();
	
	void Draw(ComPtr<ID3D11DeviceContext> deviceContext, int vertexCount);

	void Update(ComPtr<ID3D11DeviceContext> deviceContext, int x, int y, int width, shared_ptr<TerrainVertex> vertices);
};