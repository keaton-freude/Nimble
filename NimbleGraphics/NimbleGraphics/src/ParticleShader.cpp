#include "..\include\ParticleShader.h"
#include "Structs.h"
#include "ParticleVertex.h"
#include "ParticleSettings.h"
#include "Helper.h"
#include <DirectXColors.h>

using DirectX::Colors::Black;

ParticleShader::ParticleShader()
	: IShader()
{
	LOG_INFO("Particle Shader Construct!");
	this->_vsFilename = L"..\\..\\Assets\\Shaders\\ParticleShader.vs";
	this->_psFilename = L"..\\..\\Assets\\Shaders\\ParticleShader.ps";
	this->_vertexShaderEntryPoint = "ParticleVertexShader";
	this->_pixelShaderEntryPoint = "ParticlePixelShader";
	this->_vsVersion = "vs_5_0";
	this->_psVersion = "ps_5_0";
}

ParticleShader::~ParticleShader()
{
	LOG_INFO("Particle Shader Destruct!");
}

ParticleShader::ParticleShader(ComPtr<ID3D11Device> device, ParticleSettings settings)
	: IShader()
{
	LOG_INFO("Particle Shader Construct!");
	this->_vsFilename = L"..\\..\\Assets\\Shaders\\ParticleShader.vs";
	this->_psFilename = L"..\\..\\Assets\\Shaders\\ParticleShader.ps";
	this->_vertexShaderEntryPoint = "ParticleVertexShader";
	this->_pixelShaderEntryPoint = "ParticlePixelShader";
	this->_vsVersion = "vs_5_0";
	this->_psVersion = "ps_5_0";

	this->Load(device);

	CreateShaderConstants(device, settings);
}

bool ParticleShader::Load(ComPtr<ID3D11Device> device)
{
	// Do basic load work
	if (!IShader::Load(device))
	{
		cout << "IShader failed to load correctly in ParticleShader. " << endl;
		return false;
    }

	return true;
}

void ParticleShader::CreateShaderConstants(ComPtr<ID3D11Device> device, ParticleSettings settings)
{
	D3D11_BUFFER_DESC constantParticleBufferDesc;
	D3D11_SUBRESOURCE_DATA constantParticleData;

	ConstantParticleParameters parameters;

	parameters.Duration = settings.duration;
	parameters.DurationRandomness = settings.duration_randomness;
	parameters.Gravity = settings.gravity;
	parameters.EndVelocity = settings.end_velocity;
	parameters.MinColor = settings.min_color;
	parameters.MaxColor = settings.max_color;
	parameters.RotateSpeed = Vector2(settings.min_rotate_speed, settings.max_rotate_speed);
	parameters.StartSize = settings.start_size;
	parameters.EndSize = settings.end_size;

	constantParticleBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	constantParticleBufferDesc.ByteWidth = sizeof(ConstantParticleParameters);
	constantParticleBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantParticleBufferDesc.CPUAccessFlags = 0;
	constantParticleBufferDesc.MiscFlags = 0;
	constantParticleBufferDesc.StructureByteStride = 0;

	constantParticleData.pSysMem = &parameters;
	constantParticleData.SysMemPitch = 0;
	constantParticleData.SysMemSlicePitch = 0;

	device->CreateBuffer(&constantParticleBufferDesc, &constantParticleData, constantParticleParameters.GetAddressOf());

	// create the per frame buffer
	D3D11_BUFFER_DESC perFrameBufferDesc;

	perFrameBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	perFrameBufferDesc.ByteWidth = sizeof(PerFrameParticleParameters);
	perFrameBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	perFrameBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	perFrameBufferDesc.MiscFlags = 0;
	perFrameBufferDesc.StructureByteStride = 0;

	device->CreateBuffer(&perFrameBufferDesc, NULL, perFrameParticleParameters.GetAddressOf());
}

void ParticleShader::Update(const Matrix& worldMatrix, const Matrix& viewMatrix, const Matrix& projectionMatrix)
{
	this->_worldMatrix = worldMatrix;
	this->_viewMatrix = viewMatrix;
	this->_projectionMatrix = projectionMatrix;
}

bool ParticleShader::Draw(ComPtr<ID3D11DeviceContext> deviceContext, int indexCount, int startIndexLocation, int baseVertexLocation,
	Texture* texture, Vector2 viewport, float current_time, bool update)
{
	// Set the shader parameters that it will use for rendering.
	this->SetShaderParameters(deviceContext, texture->GetTexture(), viewport, current_time, update);


	// Now render the prepared buffers with the shader.
	this->RenderShader(deviceContext, indexCount, startIndexLocation, baseVertexLocation);

	return true;
}

void ParticleShader::RenderShader(ComPtr<ID3D11DeviceContext> deviceContext, int indexCount, int startIndexLocation, int baseVertexLocation)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(this->_layout.Get());

	deviceContext->VSSetShader(this->_vertexShader.Get(), NULL, 0);
	deviceContext->PSSetShader(this->_pixelShader.Get(), NULL, 0);

	auto ss = StatesHelper::GetInstance().GetStates()->LinearWrap();
	deviceContext->PSSetSamplers(0, 1, &ss);

	// indexCount, StartIndexLocation, BaseVertexLocation
	deviceContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}

void ParticleShader::UpdatePerFrameBuffer(ComPtr<ID3D11DeviceContext> deviceContext, Vector2 viewport_scale, float current_time)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	
	deviceContext->Map(this->perFrameParticleParameters.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	PerFrameParticleParameters* dataPtr;
	dataPtr = (PerFrameParticleParameters*)mappedResource.pData;
	dataPtr->CurrentTime = current_time;
	dataPtr->ViewportScale = viewport_scale;
	deviceContext->Unmap(this->perFrameParticleParameters.Get(), 0);
}

// update means we want to set the matrix buffers, can't do this in back to back calls
// so call this update=false if you are chaining draw calls within 1 logical draw
bool ParticleShader::SetShaderParameters(ComPtr<ID3D11DeviceContext> deviceContext, ComPtr<ID3D11ShaderResourceView> texture,
	Vector2 viewport, float current_time, bool update)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	deviceContext->PSSetShaderResources(0, 1, texture.GetAddressOf());

	UpdatePerFrameBuffer(deviceContext, viewport, current_time);

	deviceContext->VSSetConstantBuffers(1, 1, perFrameParticleParameters.GetAddressOf());
	deviceContext->VSSetConstantBuffers(2, 1, constantParticleParameters.GetAddressOf());

	if (update)
	{
		// Must let IShader do its Matrix Buffer copies!
		return IShader::SetShaderParameters(deviceContext);
	}

	return true;
}

void ParticleShader::GetPolygonLayout(shared_ptr<D3D11_INPUT_ELEMENT_DESC>& desc, unsigned int & numElements)
{
	desc = ParticleVertex::GetLayout(numElements);
}
