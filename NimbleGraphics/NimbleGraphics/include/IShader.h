#pragma once
#include "Typedefs.h"
#include <wrl/client.h>
#include <memory>
#include <D3Dcompiler.h>
#include "Logger.h"
#include "IShaderComponent.h"
#include <vector>
#include "ShaderComponentManager.h"

using DirectX::SimpleMath::Matrix;
using std::string;
using std::wstring;
using Microsoft::WRL::ComPtr;
using std::shared_ptr;
using std::ofstream;

class IShader
{
public:
	IShader();

	IShader(D3DDevice device, D3DDeviceContext deviceContext);

	virtual ~IShader();

	virtual bool Load() = 0;
	virtual void Draw(int indexCount) = 0;
	virtual bool SetShaderParameters() = 0;

protected:
	virtual void GetPolygonLayout(shared_ptr<D3D11_INPUT_ELEMENT_DESC>& desc, 
		unsigned int & numElements) = 0;


	virtual void SetComponents() = 0;
	ShaderComponentManager _shaderComponentManager;

protected:
	wstring _vsFilename;
	wstring _psFilename;
	string _vertexShaderEntryPoint;
	string _pixelShaderEntryPoint;
	string _vsVersion;
	string _psVersion;
	
	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3D11PixelShader> _pixelShader;

	D3DDevice _device;
	D3DDeviceContext _deviceContext;
};