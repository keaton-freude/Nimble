#include "ShaderManager.h"
#include "Logger.h"

void ShaderManager::Load(ComPtr<ID3D11Device> device)
{
	std::pair<SHADER, shared_ptr<IShader>> pair = std::pair<SHADER, shared_ptr<IShader>>(SHADER::COLOR, 
		make_shared<ColorShader>(device));

	_shaders.insert(pair);

	pair = std::pair<SHADER, shared_ptr<IShader>>(SHADER::TERRAIN,
		make_shared<TerrainShader>(device));

	_shaders.insert(pair);

	//pair = std::pair<SHADER, shared_ptr<IShader>>(SHADER::PARTICLE, make_shared<ParticleShader>(device));

	//_shaders.insert(pair);
}

ShaderManager::ShaderManager(ShaderManager&& other)
{
	*this = std::move(other);
}

ShaderManager& ShaderManager::operator=(ShaderManager&& other)
{
	if (this != &other)
	{

	}

	return *this;
}

ShaderManager::~ShaderManager()
{
	this->Shutdown();

}

ShaderManager::ShaderManager()
{
}

void ShaderManager::Shutdown()
{
	LOG_INFO("Shader manager being shut down.");
	_shaders.clear();
}
