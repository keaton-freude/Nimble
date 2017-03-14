#include "Tile.h"

Tile::Tile(unsigned int width, unsigned int height, D3DDevice device, D3DDeviceContext context, vector<wstring> texture_paths)
	: _heightmap(width, height, 1.0f), _material(std::make_shared<TileMaterial>(device, context, texture_paths)), _drawable(Mesh::CreateFromHeightmap(device, _heightmap)),
	_renderObject(_drawable, _material, Transform(Vector3::Zero, Vector3::Zero, Vector3::One)), _boundingBox()
{
	BoundingBox::CreateFromPoints(_boundingBox, Vector3(0.0f, 0.0f, 0.0f), Vector3(_heightmap.GetWidth(), 1.0f, height));
}

size_t Tile::GetWidth() const
{
	return _heightmap.GetWidth();
}

size_t Tile::GetHeight() const
{
	return _heightmap.GetHeight();
}

RayHit Tile::IsRayIntersecting(const Ray& ray)
{
	float distance;
	if (ray.Intersects(_boundingBox, distance))
	{
		return _heightmap.IsRayIntersectingVerts(ray);
	}

	return RayHit::NoHit();
}

MemoryHeightmap& Tile::GetHeightmap()
{
	return _heightmap;
}

RenderObject& Tile::GetRenderObject()
{
	return _renderObject;
}