#include "TileMaterial.h"
#include "DiffuseShader.h"

TileMaterial::TileMaterial()
	: Material(ShaderManager::GetInstance().GetShader<TileShader>(SHADER::TILE)), _device(nullptr), _deviceContext(nullptr)
{
	_shader = ShaderManager::GetInstance().GetShader<TileShader>(SHADER::TILE);
}

TileMaterial::TileMaterial(D3DDevice device, D3DDeviceContext deviceContext, vector<wstring> splatTextures)
	: Material(ShaderManager::GetInstance().GetShader<TileShader>(SHADER::TILE)), _device(device), _deviceContext(deviceContext),
	_heightmap(2, 2, 1.0f), light(Vector3(.45f, 0.0f, .45f), Vector4::One, Vector4::One), _texture(make_shared<Texture>(device, deviceContext, splatTextures[0].c_str()))
{
	_shader = ShaderManager::GetInstance().GetShader<TileShader>(SHADER::TILE);
	//_shader = ShaderManager::GetInstance().GetShader<TileShader>(SHADER::TERRAIN);
	//_splatMap.LoadFromFile(device, deviceContext, splatTextures);
	//_shader.lock().g

	HRESULT result;

	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC splatBufferDesc;

	// Add on Terrain-specific loading work
	// Create the texture sampler state.
	auto state = StatesHelper::GetInstance().GetStates()->LinearWrap();
	result = device->CreateSamplerState(&samplerDesc, &state);

	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBuffer);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&lightBufferDesc, nullptr, _lightBuffer.GetAddressOf());

	if (FAILED(result))
	{
		cout << "Failed to create light Buffer Description." << endl;
		return;
	}

	splatBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	splatBufferDesc.ByteWidth = sizeof(SplatBuffer);
	splatBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	splatBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	splatBufferDesc.MiscFlags = 0;
	splatBufferDesc.StructureByteStride = 0;

	auto hr = device->CreateBuffer(&splatBufferDesc, nullptr, _splatBuffer.GetAddressOf());
}

MemoryHeightmap& TileMaterial::GetHeightmap()
{
	return _heightmap;
}

void TileMaterial::Apply(D3DDeviceContext deviceContext)
{
	// Set the shader parameters that it will use for rendering.
	//_shader.lock()->
	_shader.lock()->SetDiffuseTexture(_texture);
	bool result = _shader.lock()->SetShaderParameters();
}
