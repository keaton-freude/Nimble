#pragma once
#include "IShader.h"
#include <vector>
#include "IShaderComponent.h"


/* 
 * BasicShader contains drawing functionality, compiling and WVPComponent
 * added. Also includes a _layout (which most if not all shaders want,
 * layout refers to the structre, naming and grouping of the data given
 * in the mesh's VBO.
 */
class BasicShader: public IShader
{
public:
	explicit BasicShader(D3DDevice device, D3DDeviceContext deviceContext);

	void Draw(int indexCount) override;
	bool Load() override;
	std::vector<shared_ptr<IShaderComponent>>& GetComponents() override;
private:
	bool Compile(ComPtr<ID3D10Blob>& vertexShaderBuffer);

protected:


	void SetComponents() override;
public:
	bool SetShaderParameters() override;
private:
	D3DInputLayout _layout;

};
