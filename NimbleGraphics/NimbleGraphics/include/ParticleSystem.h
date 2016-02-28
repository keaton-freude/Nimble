#pragma once
#include <d3d11.h>
#include "ParticleSettings.h"
#include "ParticleVertex.h"
#include "VertexBuffer.h"
#include "ParticleShader.h"
#include <memory>

using std::shared_ptr;
using std::unique_ptr;

class ParticleSystem
{
public:
	ParticleSystem(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext,
		const ParticleSettings& settings);

	void Update(const Matrix& viewMatrix, const Matrix& projectionMatrix, float dt);
	void Draw(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	int first_active_particle;
	int first_new_particle;
	int first_free_particle;
	int first_retired_particle;
	int startIndex;
	int length;
	ParticleSettings settings;
private:

	shared_ptr<Texture> texture;
	shared_ptr<ParticleVertex> particles;
	shared_ptr<VertexBuffer> vertex_buffer; // contains both VertexBuffer and IndexBuffer
	unique_ptr<ParticleShader> particle_shader;

	float current_time;
	int draw_counter;
	float timeLeftOver;

	// methods
	void Initialize();
	void LoadContent(ComPtr<ID3D11Device> device);
	void LoadParticleEffect();
	void RetireActiveParticles();
	void FreeRetiredParticles();
	void AddNewParticlesToVertexBuffer(ComPtr<ID3D11DeviceContext> deviceContext);
	void AddParticle(Vector3 position);

};