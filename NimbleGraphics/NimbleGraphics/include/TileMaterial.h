#pragma once
#include "Material.h"
#include "MemoryHeightmap.h"
#include "Light.h"
#include "SplatMap.h"

class TileMaterial: public Material
{
public:
	TileMaterial();

	explicit TileMaterial(D3DDevice device, D3DDeviceContext deviceContext, vector<wstring> splatTextures);
	MemoryHeightmap& GetHeightmap();
	void Apply(D3DDeviceContext deviceContext) override;
	//void Update()
private:
	MemoryHeightmap _heightmap;
	SplatMap _splatMap;
	Light light;

	// D3D Resources
	D3DBuffer _lightBuffer;
	D3DBuffer _splatBuffer;
	D3DSamplerState _sampleState;

	D3DDevice _device;
	D3DDeviceContext _deviceContext;
};
