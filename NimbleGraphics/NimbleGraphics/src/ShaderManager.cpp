#include "ShaderManager.h"
#include "DiffuseShader.h"
#include "TileShader.h"
#include "Logger.h"

ShaderManager::ShaderManager()
{
}

void ShaderManager::Load(ComPtr<ID3D11Device> device, D3DDeviceContext deviceContext)
{
	//std::pair<SHADER, shared_ptr<IShader>> pair = std::pair<SHADER, shared_ptr<IShader>>(SHADER::COLOR,
//	                                                                                     make_shared<ColorShader>(device, deviceContext));

	//_shaders.insert(pair);

	//pair = std::pair<SHADER, shared_ptr<IShader>>(SHADER::TERRAIN,
	 //                                             make_shared<TerrainShader>(device, deviceContext));

	//_shaders.insert(pair);

	auto pair = std::pair<SHADER, shared_ptr<IShader>>(SHADER::DIFFUSE,
		make_shared<DiffuseShader>(device, deviceContext));

	pair.second->Load();
	_shaders.insert(pair);

	pair = std::pair<SHADER, shared_ptr<IShader>>(SHADER::TILE,
		make_shared<TileShader>(device, deviceContext));

	pair.second->Load();
	_shaders.insert(pair);

	//pair = std::pair<SHADER, shared_ptr<IShader>>(SHADER::PARTICLE, make_shared<ParticleShader>(device));

	//_shaders.insert(pair);
}

ShaderManager::~ShaderManager()
{
	this->Shutdown();
}

void ShaderManager::Shutdown()
{
	LOG_INFO("Shader manager being shut down.");
	_shaders.clear();
}