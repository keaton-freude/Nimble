#pragma once
#include "Material.h"
#include "TileShader.h"
#include "Light.h"


#include <vector>

using std::vector;

class TileMaterial: public Material
{
public:
	TileMaterial();

	explicit TileMaterial(D3DDevice device, D3DDeviceContext deviceContext, vector<wstring> splatTextures);
	void Apply(D3DDeviceContext deviceContext) override;
	//void Update()
private:
	shared_ptr<Texture> _texture;
	//SplatMap _splatMap;
	Light light;

	// D3D Resources
	D3DBuffer _splatBuffer;

	D3DDevice _device;
	D3DDeviceContext _deviceContext;
	std::weak_ptr<TileShader> _shader;
};
