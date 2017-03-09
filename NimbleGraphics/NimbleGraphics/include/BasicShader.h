#pragma once
#include "IShader.h"


/* 
 * BasicShader contains drawing functionality, compiling and WVPComponent
 * added. Also includes a _layout (which most if not all shaders want,
 * layout refers to the structre, naming and grouping of the data given
 * in the mesh's VBO.
 */
class BasicShader: public IShader
{
public:
	BasicShader() = default;
	BasicShader(D3DDevice device, D3DDeviceContext deviceContext);

	void Draw(int indexCount) override;
	bool Load() override;
	bool SetShaderParameters() override;

protected:
	void SetComponents() override;
	void GetPolygonLayout(shared_ptr<D3D11_INPUT_ELEMENT_DESC>& desc, unsigned& numElements) override;

private:
	bool Compile(ComPtr<ID3D10Blob>& vertexShaderBuffer);
	D3DInputLayout _layout;

};
