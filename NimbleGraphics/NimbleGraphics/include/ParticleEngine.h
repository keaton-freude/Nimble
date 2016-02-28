#pragma once
#include <vector>
#include "ParticleEffect.h"
#include "ParticleSettings.h"
#include "Logger.h"
#include <memory.h>
using std::vector;
using std::unique_ptr;

class ParticleEngine
{
public:
	ParticleEngine();
	~ParticleEngine();

	void Update(const Matrix& viewMatrix, const Matrix& projectionMatrix, float dt);
	void Draw(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);

	void Add(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext,
		vector<ParticleSettings>& settings);

	void Clear();
private:
	float _dt;
	vector<unique_ptr<ParticleEffect>> _effects;
};