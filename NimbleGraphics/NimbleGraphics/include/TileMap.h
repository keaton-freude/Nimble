#pragma once
#include "Tile.h"
#include <unordered_map>
#include "RayHit.h"
#include "../../../NimbleEditor/d3drenderwidget.h"

using std::vector;

class TileMap
{
public:
	explicit TileMap(const int width, const int height)
		: _map(width * height), _visibleMap(width * height)
	{}

	void Load(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);

	void Draw(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext,
		const Matrix& viewMatrix, const Matrix& projectionMatrix, const Light& light, const Frustum& frustum);

	void SplatTexture(D3DDevice device, D3DDeviceContext deviceContext, Vector3 location, float radius, int intensity, int texture_number);

	Tile* GetRayIntersectingTile(const Ray& r) const
	{
		for (const auto& tile: _visibleMap)
		{
			auto rayhit = tile->IsRayIntersecting(r);
			if (rayhit.hit)
			{
				return tile;
			}
		}
		return nullptr;
	}

	~TileMap() = default;

private:
	void LoadChunks(ComPtr<ID3D11Device> device);
	void UpdateChunks(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);

	// Container for all map tiles.
	vector<Tile> _map;
	// Cache for visible tiles found in the frustrum.
	vector<Tile*> _visibleMap;
};
