#pragma once
#include "Typedefs.h"
#include "IShaderComponent.h"
#include <vector>
#include "ShaderComponentManager.h"

using std::string;
using std::wstring;
using std::ofstream;

class IShader
{
public:
	IShader()
		: _device(nullptr), _deviceContext(nullptr)
	{
		
	}

	IShader(D3DDevice device, D3DDeviceContext deviceContext)
		: _shaderComponentManager(), _device(device), _deviceContext(deviceContext)
	{
		
	}

	virtual ~IShader() = default;

	virtual bool Load() = 0;
	virtual void Draw(int indexCount) = 0;
	virtual bool SetShaderParameters() = 0;

	std::vector<shared_ptr<IShaderComponent>>& GetComponents()
	{
		return _shaderComponentManager.GetComponents();
	}

protected:
	virtual void GetPolygonLayout(shared_ptr<D3D11_INPUT_ELEMENT_DESC>& desc, 
		unsigned int & numElements) = 0;


	virtual void SetComponents() = 0;

	ShaderComponentManager _shaderComponentManager;

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