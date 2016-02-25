#pragma once

#include "IShader.h"
#include "Light.h"
#include <d3d11.h>
#include "Structs.h"
#include "Helper.h"
#include <memory>

using std::shared_ptr;
using Microsoft::WRL::ComPtr;

class TerrainShader : public IShader
{
public:
	TerrainShader();
	TerrainShader(ComPtr<ID3D11Device> device);
	TerrainShader(TerrainShader&& other);
	TerrainShader& operator=(TerrainShader&& other);
	~TerrainShader();

	// IResource Load
	bool Load(ComPtr<ID3D11Device> device) override;

	// IShader Draw
	bool Draw(ComPtr<ID3D11DeviceContext> deviceContext, int indexCount, Matrix worldMatrix,
		shared_ptr<Matrix> viewMatrix, shared_ptr<Matrix> projectionMatrix, shared_ptr<Light> light, ComPtr<ID3D11ShaderResourceView> grass,
		ComPtr<ID3D11ShaderResourceView> slope, ComPtr<ID3D11ShaderResourceView> rock);

	void RenderShader(ComPtr<ID3D11DeviceContext>, int);
	bool SetShaderParameters(ComPtr<ID3D11DeviceContext> deviceContext, shared_ptr<Light> light, ComPtr<ID3D11ShaderResourceView> grass, ComPtr<ID3D11ShaderResourceView> slope,
		ComPtr<ID3D11ShaderResourceView> rock);
private:
	virtual void GetPolygonLayout(shared_ptr<D3D11_INPUT_ELEMENT_DESC>& desc, unsigned int & numElements) override;

	ComPtr<ID3D11Buffer> _lightBuffer;
	ComPtr<ID3D11SamplerState> _sampleState;
};