#pragma once
#include "ParticleSystem.h"
#include <vector>
#include <memory>

using std::unique_ptr;
using std::vector;

class ParticleEffect
{
public:
	ParticleEffect()
	{
		
	}

	ParticleEffect(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext,
		vector<ParticleSettings>& systems_to_create)
	{
		for (auto const& setting : systems_to_create)
		{
			_systems.push_back(std::make_unique<ParticleSystem>(device, deviceContext, setting));
		}
	}

	~ParticleEffect()
	{
		
	}

	void Update(const Matrix& viewMatrix, const Matrix& projectionMatrix, float dt)
	{
		for (auto &system : _systems)
		{
			system->Update(viewMatrix, projectionMatrix, dt);
		}
	}

	void Draw(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
	{
		for (auto &system : _systems)
		{
			system->Draw(device, deviceContext);
		}
	}

private:
	vector<unique_ptr<ParticleSystem>> _systems;
};