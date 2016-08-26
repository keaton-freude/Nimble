#pragma once
#include <vector>
#include "ParticleEffect.h"
#include "ParticleSettings.h"
#include "Logger.h"
using std::vector;
using std::unique_ptr;

class ParticleEngine
{
public:
	ParticleEngine()
		: _dt(0), _effects()
	{
		LOG_INFO("-- ParticleEngine Default C'tor --");
	}

	~ParticleEngine()
	{
		LOG_INFO("Particle Engine Destruct.");
	}

	void Update(const Matrix& viewMatrix, const Matrix& projectionMatrix, float dt)
	{
		//_dt += dt;

		// if we don't have any effects, we can reset dt. might save us from losing
		// float precision if we take advantage of times when no effects are being rendered

		for (auto & effect : _effects)
		{
			effect->Update(viewMatrix, projectionMatrix, dt);
		}
	}

	void Draw(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
	{
		for (auto & effect : _effects)
		{
			effect->Draw(device, deviceContext);
		}
	}

	void Add(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext,
		vector<ParticleSettings>& settings)
	{
		_effects.push_back(std::make_unique<ParticleEffect>(device, deviceContext, settings));
	}

	void Clear()
	{
		_effects.clear();
	}
private:
	float _dt;
	vector<unique_ptr<ParticleEffect>> _effects;
};