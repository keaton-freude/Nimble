#pragma once
#include <unordered_map>
#include <wrl\client.h>
#include <iostream>
#include <memory>

#include "IShader.h"
#include "ColorShader.h"
#include "TerrainShader.h"

using std::shared_ptr;
using std::unordered_map;
using std::cout;
using std::endl;
using Microsoft::WRL::ComPtr;

enum SHADER
{
	COLOR = 0,
	TERRAIN = 1,
	PARTICLE = 2
};

class ShaderManager: public Singleton<ShaderManager>
{
public:
	ShaderManager()
	{

	}

	void Load(ComPtr<ID3D11Device> device)
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

	~ShaderManager()
	{
		this->Shutdown();
		
	}

	void Shutdown()
	{
		LOG_INFO("Shader manager being shut down.");
		_shaders.clear();
	}

	template<typename T>
    T* GetShader(SHADER shader)
	{
		auto element = _shaders.find(shader);
		if (element == _shaders.end())
		{
			return nullptr;
		}

		T* resource = dynamic_cast<T*>(element->second.get());

		if (resource)
		{
			return resource;
		}

		return nullptr;
	}
protected:
	ShaderManager(ShaderManager const& other) = delete;
	void operator=(ShaderManager const&) = delete;

	unordered_map<SHADER, shared_ptr<IShader>> _shaders;
};