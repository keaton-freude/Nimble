#include "TileShader.h"
#include "DiffuseShaderComponent.h"
#include "SamplerStateShaderComponent.h"
#include "WVPShaderComponent.h"
#include "LightShaderComponent.h"

TileShader::TileShader(D3DDevice device, D3DDeviceContext deviceContext)
	: BasicShader(device, deviceContext)
{
	this->_vsFilename = L"..\\Assets\\Shaders\\TileShader.vs";
	this->_psFilename = L"..\\Assets\\Shaders\\TileShader.ps";
	this->_vertexShaderEntryPoint = "vertexMain";
	this->_pixelShaderEntryPoint = "pixelMain";
	this->_vsVersion = "vs_5_0";
	this->_psVersion = "ps_5_0";
}

TileShader::~TileShader()
{
}

void TileShader::SetDiffuseTexture(shared_ptr<Texture> texture)
{
	auto& comps = this->GetComponents();
	auto ptr = static_cast<DiffuseShaderComponent*>(comps[2].get());
	ptr->SetTexture(texture);
}

void TileShader::SetComponents()
{
	_shaderComponentManager.Add(make_shared<WVPShaderComponent>());
	_shaderComponentManager.Add(make_shared<SamplerStateShaderComponent>(StatesHelper::GetInstance().GetStates()->LinearWrap()));
	_shaderComponentManager.Add(make_shared<DiffuseShaderComponent>());
	_shaderComponentManager.Add(make_shared<LightShaderComponent>());
}

void TileShader::GetPolygonLayout(shared_ptr<D3D11_INPUT_ELEMENT_DESC>& desc, unsigned int& numElements)
{
	numElements = 3;

	desc = shared_ptr<D3D11_INPUT_ELEMENT_DESC>(new D3D11_INPUT_ELEMENT_DESC[numElements]);

	desc.get()[0].SemanticName = "POSITION";
	desc.get()[0].SemanticIndex = 0;
	desc.get()[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	desc.get()[0].InputSlot = 0;
	desc.get()[0].AlignedByteOffset = 0;
	desc.get()[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	desc.get()[0].InstanceDataStepRate = 0;

	desc.get()[1].SemanticName = "TEXCOORD";
	desc.get()[1].SemanticIndex = 0;
	desc.get()[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	desc.get()[1].InputSlot = 0;
	desc.get()[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	desc.get()[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	desc.get()[1].InstanceDataStepRate = 0;

	desc.get()[2].SemanticName = "NORMAL";
	desc.get()[2].SemanticIndex = 0;
	desc.get()[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	desc.get()[2].InputSlot = 0;
	desc.get()[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	desc.get()[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	desc.get()[2].InstanceDataStepRate = 0;
}
