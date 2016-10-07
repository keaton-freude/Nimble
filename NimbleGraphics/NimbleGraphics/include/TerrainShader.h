#pragma once

#include "Typedefs.h"
#include "IShader.h"
#include "Light.h"
#include "Structs.h"


class TerrainShader : public IShader
{
public:
	TerrainShader();

	TerrainShader(ComPtr<ID3D11Device> device);

	virtual ~TerrainShader();

	// IResource Load
	bool Load(ComPtr<ID3D11Device> device) override;

	// IShader Draw
	bool Draw(ComPtr<ID3D11DeviceContext> deviceContext, int indexCount, const Matrix& worldMatrix,
	          const Matrix& viewMatrix, const Matrix& projectionMatrix, const Light& light, ComPtr<ID3D11ShaderResourceView> grass,
	          ComPtr<ID3D11ShaderResourceView> slope, ComPtr<ID3D11ShaderResourceView> rock, ComPtr<ID3D11ShaderResourceView> splat);

	void GetPolygonLayout(shared_ptr<D3D11_INPUT_ELEMENT_DESC>& desc, unsigned int& numElements) override;

	void RenderShader(ComPtr<ID3D11DeviceContext> deviceContext, int indexCount);

	bool SetShaderParameters(ComPtr<ID3D11DeviceContext> deviceContext, const Light& light, ComPtr<ID3D11ShaderResourceView> grass, ComPtr<ID3D11ShaderResourceView> slope,
	                         ComPtr<ID3D11ShaderResourceView> rock, ComPtr<ID3D11ShaderResourceView> splat);
private:

	ComPtr<ID3D11Buffer> _lightBuffer;
	ComPtr<ID3D11SamplerState> _sampleState;
};