#pragma once
#include "MemoryHeightmap.h"
#include "D3D11Object.h"
#include "TerrainMesh.h"
#include <SimpleMath.h>
#include "Mesh.h"
#include "TileMaterial.h"
#include "RenderObject.h"

using DirectX::BoundingBox;

class Tile : public D3D11Object
{
public: 
	Tile(unsigned int width, unsigned int height, D3DDevice device, D3DDeviceContext context, vector<wstring> texture_paths);
	~Tile() = default;

	//void SmoothHeightmapAdd(const Vector3& location, const float& radius, const float& intensity, ComPtr<ID3D11DeviceContext> deviceContext,
	//	ComPtr<ID3D11Device> device);

	size_t GetWidth() const;
	size_t GetHeight() const;
	RayHit IsRayIntersecting(const Ray& ray);
	MemoryHeightmap& Tile::GetHeightmap();
	RenderObject& GetRenderObject();

private:

	// defines the topographical features of the tile
	MemoryHeightmap _heightmap;
	shared_ptr<TileMaterial> _material;
	shared_ptr<Mesh> _drawable;
	RenderObject _renderObject;
	BoundingBox _boundingBox;
};
