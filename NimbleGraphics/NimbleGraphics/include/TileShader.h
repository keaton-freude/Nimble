#pragma once
#include "BasicShader.h"
#include "Texture.h"

class TileShader : public BasicShader
{
public:
	TileShader() = default;

	TileShader(D3DDevice device, D3DDeviceContext deviceContext);

	~TileShader();

	void SetDiffuseTexture(shared_ptr<Texture> texture);

protected:
	void SetComponents() override;
	void GetPolygonLayout(shared_ptr<D3D11_INPUT_ELEMENT_DESC>& desc, unsigned int& numElements) override;
	

private:
};
