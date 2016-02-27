#pragma once
#include <d3d11.h>
#include <fstream>
#include <wrl\client.h>
#include <memory>
#include <d3dcompiler.h>
#include <SimpleMath.h>

#include "Logger.h"
#include "IResource.h"

using DirectX::SimpleMath::Matrix;
using std::string;
using std::wstring;
using Microsoft::WRL::ComPtr;
using std::shared_ptr;
using std::ofstream;

class IShader : public IResource
{
public:
	IShader();
	~IShader();
	IShader(IShader&& other);
	IShader& operator=(IShader&& other);

    bool Load(ComPtr<ID3D11Device> device);
    void Unload();

	void SetWorldMatrix(Matrix worldMatrix);
	void SetViewMatrix(Matrix viewMatrix);
	void SetProjectionMatrix(Matrix projectionMatrix);

protected:
	virtual D3D11_BUFFER_DESC GetMatrixBufferDescription();
	virtual void GetPolygonLayout(shared_ptr<D3D11_INPUT_ELEMENT_DESC>& desc, 
		unsigned int & numElements) = 0;

	virtual bool SetShaderParameters(ComPtr<ID3D11DeviceContext> deviceContext);
	virtual bool Init(ComPtr<ID3D11Device> device);


	virtual void Shutdown();

protected:
	struct MatrixBuffer
	{
		Matrix world;
		Matrix view;
		Matrix projection;
	};
protected:
	wstring _vsFilename;
	wstring _psFilename;
	string _vertexShaderEntryPoint;
	string _pixelShaderEntryPoint;
	string _vsVersion;
	string _psVersion;
	
	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3D11PixelShader> _pixelShader;
	ComPtr<ID3D11InputLayout> _layout;
	ComPtr<ID3D11Buffer> _matrixBuffer;

	Matrix _worldMatrix;
	Matrix _viewMatrix;
	Matrix _projectionMatrix;
};