#pragma once

#include "Typedefs.h"
#include <memory>
#include "IShader.h"

using std::shared_ptr;

class ColorShader : public IShader
{
public:
	ColorShader();

	explicit ColorShader(ComPtr<ID3D11Device> device);

	virtual ~ColorShader();


	bool Load(ComPtr<ID3D11Device> device) override;

	bool Draw(ComPtr<ID3D11DeviceContext> deviceContext, int indexCount, MatrixRef world, MatrixRef view,
	          MatrixRef projection);

	ComPtr<ID3D11VertexShader> GetVertexShader();

private:
	void GetPolygonLayout(shared_ptr<D3D11_INPUT_ELEMENT_DESC>& desc,
	                      unsigned int& numElements) override;

	void RenderShader(ComPtr<ID3D11DeviceContext> deviceContext, int indexCount);
};