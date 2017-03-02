#pragma once
#include "Material.h"
#include "MemoryHeightmap.h"
#include "Light.h"
#include "SplatMap.h"
#include "TileShader.h"

class TileMaterial: public Material
{
public:
	TileMaterial();

	explicit TileMaterial(D3DDevice device, D3DDeviceContext deviceContext, vector<wstring> splatTextures);
	MemoryHeightmap& GetHeightmap();
	void Apply(D3DDeviceContext deviceContext) override;
	//void Update()
private:
	shared_ptr<Texture> _texture;
	MemoryHeightmap _heightmap;
	//SplatMap _splatMap;
	Light light;

	// D3D Resources
	D3DBuffer _splatBuffer;

	D3DDevice _device;
	D3DDeviceContext _deviceContext;
	std::weak_ptr<TileShader> _shader;
};
