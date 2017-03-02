#include "TileMaterial.h"
#include "DiffuseShader.h"

TileMaterial::TileMaterial()
	: Material(ShaderManager::GetInstance().GetShader<TileShader>(SHADER::TILE)), _device(nullptr), _deviceContext(nullptr)
{
	_shader = ShaderManager::GetInstance().GetShader<TileShader>(SHADER::TILE);
}

TileMaterial::TileMaterial(D3DDevice device, D3DDeviceContext deviceContext, vector<wstring> splatTextures)
	: Material(ShaderManager::GetInstance().GetShader<TileShader>(SHADER::TILE)), _device(device), _deviceContext(deviceContext),
	_heightmap(16, 16, 1.0f), light(Vector3(.45f, 0.0f, .45f), Vector4::One, Vector4::One), _texture(make_shared<Texture>(device, deviceContext, splatTextures[0].c_str()))
{
	_shader = ShaderManager::GetInstance().GetShader<TileShader>(SHADER::TILE);
	//_shader = ShaderManager::GetInstance().GetShader<TileShader>(SHADER::TERRAIN);
	//_shader.lock().g
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
