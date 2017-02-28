#pragma once
#include "Typedefs.h"
#include <unordered_map>
#include <iostream>

#include "IShader.h"

using std::unordered_map;
using std::cout;
using std::endl;

enum SHADER
{
	COLOR = 0,
	TERRAIN = 1,
	PARTICLE = 2,
	DIFFUSE = 3,
	TILE = 4
};

class ShaderManager: public Singleton<ShaderManager>
{
public:
	ShaderManager();

	void Load(ComPtr<ID3D11Device> device, D3DDeviceContext deviceContext);

	~ShaderManager();

	void Shutdown();

	template<typename T>
    shared_ptr<T> GetShader(SHADER shader)
	{
		auto element = _shaders.find(shader);
		if (element == _shaders.end())
		{
			return nullptr;
		}

		return std::dynamic_pointer_cast<T>(element->second);
	}
protected:
	ShaderManager(ShaderManager const& other) = delete;
	void operator=(ShaderManager const&) = delete;

	unordered_map<SHADER, shared_ptr<IShader>> _shaders;
};