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

class ShaderManager
{
public:
	static ShaderManager& GetInstance()
	{
		static ShaderManager instance;
		return instance;
	}

	void Load(ComPtr<ID3D11Device> device);

	ShaderManager(ShaderManager&& other);
	ShaderManager& operator=(ShaderManager&& other);

	~ShaderManager();

	void Shutdown();

	template<typename T>
    T* GetShader(SHADER shader);
private:
	ShaderManager();

	ShaderManager(ShaderManager const& other) = delete;
	void operator=(ShaderManager const&) = delete;

	unordered_map<SHADER, shared_ptr<IShader>> _shaders;
};

template<typename T>
inline T* ShaderManager::GetShader(SHADER shader)
{
	auto element = _shaders.find(shader);
	if (element == _shaders.end())
	{
		return nullptr;
	}

	T* resource = dynamic_cast<T*>(element->second.get());

	if (resource)
		return resource;
	return nullptr;
}