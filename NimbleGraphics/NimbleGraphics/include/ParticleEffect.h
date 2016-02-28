#pragma once
#include "ParticleSystem.h"
#include <vector>
#include <memory>

using std::unique_ptr;
using std::vector;

class ParticleEffect
{
public:
	ParticleEffect();
	ParticleEffect(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext,
		vector<ParticleSettings>& systems_to_create);
	~ParticleEffect();

	void Update(const Matrix& viewMatrix, const Matrix& projectionMatrix, float dt);
	void Draw(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);

private:
	vector<unique_ptr<ParticleSystem>> _systems;
};