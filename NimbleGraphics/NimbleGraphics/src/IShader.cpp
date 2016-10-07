#include "IShader.h"

IShader::IShader()
{
	LOG_INFO("IShader default c'tor");
}

IShader::~IShader()
{
	LOG_INFO("IShader Destruct!");
}

bool IShader::Load(ComPtr<ID3D11Device> device)
{
	return this->Init(device);
}

void IShader::Unload()
{
	Shutdown();
}

void IShader::SetWorldMatrix(const Matrix& worldMatrix)
{
	this->_worldMatrix = worldMatrix;
}

void IShader::SetViewMatrix(const Matrix& viewMatrix)
{
	this->_viewMatrix = viewMatrix;
}

void IShader::SetProjectionMatrix(const Matrix& projectionMatrix)
{
	this->_projectionMatrix = projectionMatrix;
}

D3D11_BUFFER_DESC IShader::GetMatrixBufferDescription()
{
	D3D11_BUFFER_DESC matrixBufferDesc;

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	return matrixBufferDesc;
}

bool IShader::SetShaderParameters(ComPtr<ID3D11DeviceContext> deviceContext)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	result = deviceContext->Map(this->_matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	if (FAILED(result))
	{
		return false;
	}

	MatrixBuffer* dataPtr;

	dataPtr = (MatrixBuffer*)mappedResource.pData;

	_worldMatrix = _worldMatrix.Transpose();
	_viewMatrix = _viewMatrix.Transpose();
	_projectionMatrix = _projectionMatrix.Transpose();

	// Copy the matrices into the constant buffer.
	dataPtr->world = _worldMatrix;
	dataPtr->view = _viewMatrix;
	dataPtr->projection = _projectionMatrix;

	deviceContext->Unmap(this->_matrixBuffer.Get(), 0);
	deviceContext->VSSetConstantBuffers(0, 1, _matrixBuffer.GetAddressOf());

	return true;
}

bool IShader::Init(ComPtr<ID3D11Device> device)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;

	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Compile the vertex shader code.
	result = D3DCompileFromFile(_vsFilename.c_str(), NULL, NULL, _vertexShaderEntryPoint.c_str(), _vsVersion.c_str(),
	                            D3D10_SHADER_ENABLE_STRICTNESS, 0,
	                            &vertexShaderBuffer, &errorMessage);

	if (FAILED(result))
	{
		if (errorMessage)
		{
			char* compileErrors = (char*)(errorMessage->GetBufferPointer());
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
	result = D3DCompileFromFile(_psFilename.c_str(), NULL, NULL, _pixelShaderEntryPoint.c_str(), _psVersion.c_str(),
	                            D3D10_SHADER_ENABLE_STRICTNESS, 0,
	                            &pixelShaderBuffer, &errorMessage);

	if (FAILED(result))
	{
		if (errorMessage)
		{
			char* compileErrors = (char*)(errorMessage->GetBufferPointer());
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
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
	                                    vertexShaderBuffer->GetBufferSize(), NULL, _vertexShader.GetAddressOf());
	if (FAILED(result))
	{
		LOG_ERROR("Couldn't create vertex shader.");
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
	                                   pixelShaderBuffer->GetBufferSize(), NULL, _pixelShader.GetAddressOf());

	if (FAILED(result))
	{
		LOG_ERROR("Couldn't create pixel shader.");
		return false;
	}

	unsigned int numElements;
	shared_ptr<D3D11_INPUT_ELEMENT_DESC> polygonLayout;

	this->GetPolygonLayout(polygonLayout, numElements);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout.get(), numElements, vertexShaderBuffer->GetBufferPointer(),
	                                   vertexShaderBuffer->GetBufferSize(), _layout.GetAddressOf());
	if (FAILED(result))
	{
		LOG_ERROR("Failed to create vertex input layout.");
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	D3D11_BUFFER_DESC matrixBufferDesc = this->GetMatrixBufferDescription();

	// Create the constant buffer pointer so we can access the vertex shader constant buffer
	// from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, _matrixBuffer.GetAddressOf());
	if (FAILED(result))
	{
		LOG_ERROR("Failed to create Matrix Buffer in IShader");
		return false;
	}
	LOG_INFO("Created Matrix Buffer successfully.");

	return true;
}

void IShader::Shutdown()
{
}
