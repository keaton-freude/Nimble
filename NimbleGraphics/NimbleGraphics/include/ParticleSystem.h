#pragma once
#include <d3d11.h>
#include "ParticleSettings.h"
#include "ParticleVertex.h"
#include "Mesh.h"
#include "ParticleShader.h"
#include <memory>
#include "Helper.h"
#include <DirectXColors.h>

using std::shared_ptr;
using std::unique_ptr;

class ParticleSystem
{
public:
	ParticleSystem(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, const ParticleSettings& settings);

	void Update(const Matrix& viewMatrix, const Matrix& projectionMatrix, float dt);

	void Draw(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);

private:
	// fields
	unique_ptr<Texture> texture;
	unique_ptr<ParticleVertex[]> particles;
	shared_ptr<Mesh> vertex_buffer; // contains both Mesh and IndexBuffer
	unique_ptr<ParticleShader> particle_shader;

	ParticleSettings settings;
	float system_time;
	int draw_counter;
	float timeLeftOver;
	int first_active_particle;
	int first_new_particle;
	int first_free_particle;
	int first_retired_particle;
	int startIndex;
	int length;
	float timeBetweenParticles;

	// methods
	void Initialize();


	void LoadContent(ComPtr<ID3D11Device> device);

	void RetireActiveParticles();

	void FreeRetiredParticles();

	void AddNewParticlesToVertexBuffer(ComPtr<ID3D11DeviceContext> deviceContext);

	void AddParticle(Vector3 position);
};