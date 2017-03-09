#include "BasicShader.h"
#include "WVPShaderComponent.h"
#include <d3dcompiler.h>

BasicShader::BasicShader(D3DDevice device, D3DDeviceContext deviceContext)
	: IShader(device, deviceContext)
{
}

void BasicShader::Draw(int indexCount)
{
	// Set the vertex input layout.
	_deviceContext->IASetInputLayout(this->_layout.Get());

	// Set the vertex and pixel shaders that will be used to render this triangle.
	_deviceContext->VSSetShader(this->_vertexShader.Get(), nullptr, 0);
	_deviceContext->PSSetShader(this->_pixelShader.Get(), nullptr, 0);

	// Draw the triangle.
	_deviceContext->DrawIndexed(indexCount, 0, 0);
}

bool BasicShader::Load()
{
	HRESULT result;

	this->SetComponents();

	_shaderComponentManager.Load(_device, _deviceContext);

	unsigned int numElements;
	shared_ptr<D3D11_INPUT_ELEMENT_DESC> polygonLayout;
	this->GetPolygonLayout(polygonLayout, numElements);

	ComPtr<ID3D10Blob> vertexShaderBuffer;

	if (!this->Compile(vertexShaderBuffer))
	{
		LOG_ERROR("Failed to compile shaders...");
		return false;
	}

	// Create the vertex input layout.
	result = _device->CreateInputLayout(polygonLayout.get(), numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), _layout.GetAddressOf());

	if (FAILED(result))
	{
		LOG_ERROR("Failed to create vertex input layout.");
		return false;
	}

	return true;
}

bool BasicShader::Compile(ComPtr<ID3D10Blob>& vertexShaderBuffer)
{
	HRESULT result;
	ComPtr<ID3D10Blob> pixelShaderBuffer;
	ComPtr<ID3D10Blob> errorMessage;

	// Initialize the pointers this function will use to null.
	errorMessage = nullptr;
	vertexShaderBuffer = nullptr;
	pixelShaderBuffer = nullptr;

	// Compile the vertex shader code.
	result = D3DCompileFromFile(_vsFilename.c_str(), nullptr, nullptr, _vertexShaderEntryPoint.c_str(), _vsVersion.c_str(),
		D3D10_SHADER_ENABLE_STRICTNESS, 0,
		vertexShaderBuffer.GetAddressOf(), errorMessage.GetAddressOf());

	if (FAILED(result))
	{
		if (errorMessage)
		{
			char* compileErrors = static_cast<char*>(errorMessage->GetBufferPointer());
			size_t errorBufferSize = errorMessage->GetBufferSize();

			for (unsigned int i = 0; i < errorBufferSize; ++i)
			{
				cout << compileErrors[i];
			}
		}
		else
		{
			std::wcout << "Could not find shader: " << _vsFilename << endl;
		}
		return false;
	}

	// Compile the pixel shader code.
	result = D3DCompileFromFile(_psFilename.c_str(), nullptr, nullptr, _pixelShaderEntryPoint.c_str(), _psVersion.c_str(),
		D3D10_SHADER_ENABLE_STRICTNESS, 0,
		pixelShaderBuffer.GetAddressOf(), errorMessage.GetAddressOf());

	if (FAILED(result))
	{
		if (errorMessage)
		{
			auto compileErrors = static_cast<char*>(errorMessage->GetBufferPointer());
			size_t errorBufferSize = errorMessage->GetBufferSize();

			for (size_t i = 0; i < errorBufferSize; ++i)
			{
				cout << compileErrors[i];
			}
		}
		else
		{
			cout << "Could not find shader: " << _vsFilename.c_str() << endl;
		}
		return false;
	}

	// Create the vertex shader from the buffer.
	result = _device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), nullptr, _vertexShader.GetAddressOf());
	if (FAILED(result))
	{
		LOG_ERROR("Couldn't create vertex shader.");
		return false;
	}

	// Create the pixel shader from the buffer.
	result = _device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(), nullptr, _pixelShader.GetAddressOf());

	if (FAILED(result))
	{
		LOG_ERROR("Couldn't create pixel shader.");
		return false;
	}

	return true;
}

void BasicShader::SetComponents()
{
	// create components
	_shaderComponentManager.Add(make_shared<WVPShaderComponent>());
}

bool BasicShader::SetShaderParameters()
{
	_shaderComponentManager.Apply(_device, _deviceContext);

	return true;
}

void BasicShader::GetPolygonLayout(shared_ptr<D3D11_INPUT_ELEMENT_DESC>& desc, unsigned& numElements)
{
}
