#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include <wrl\client.h>
#include <memory>
#include <fstream>

#include "IResource.h"
#include "IShader.h"
#include "Structs.h"

using namespace std;
using Microsoft::WRL::ComPtr;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Color;
using std::shared_ptr;


class ColorShader : public IShader
{
public:
	ColorShader();
	ColorShader(ComPtr<ID3D11Device> device);
	~ColorShader();

	ColorShader(ColorShader&& other);
	ColorShader& operator=(ColorShader&& other);
	bool Load(ComPtr<ID3D11Device> device) override;

	bool Draw(ComPtr<ID3D11DeviceContext>, int, Matrix world, shared_ptr<Matrix> view, 
		shared_ptr<Matrix> projection);

	ComPtr<ID3D11VertexShader> GetVertexShader();

private:
	void GetPolygonLayout(shared_ptr<D3D11_INPUT_ELEMENT_DESC>& desc,
		unsigned int & numElements) override;
	void RenderShader(ComPtr<ID3D11DeviceContext> deviceContext, int indexCount);
};