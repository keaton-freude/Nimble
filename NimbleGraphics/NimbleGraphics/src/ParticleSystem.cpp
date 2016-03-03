#include "ParticleSystem.h"
#include "Helper.h"

#include <DirectXColors.h>

using DirectX::Colors::Black;

ParticleSystem::ParticleSystem(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, const ParticleSettings& settings)
	: first_active_particle(0), first_free_particle(0), first_new_particle(0), first_retired_particle(0), current_time(0.0f),
	startIndex(0), length(0), timeLeftOver(0.0f), settings(settings)
{
	// each particle system needs it own particle shader because we rely on
	// constant shader values, and we can't stomp over another particle system's
	// cbuffer space (alternative is to re-write these values per frame, instead of per
	// lifetime of the system).

	particle_shader.reset(new ParticleShader(device, settings));

	texture = std::make_shared<Texture>(device, deviceContext, settings.texture_name);
	Initialize();
	LoadContent(device);
}

void ParticleSystem::Update(const Matrix& viewMatrix, const Matrix& projectionMatrix, float dt)
{
	current_time += dt;

	float timeBetweenParticles = 1.0f / settings.particles_per_second;

	if (dt > 0)
	{
		float timeToSpend = timeLeftOver + dt;

		float currentTime = -timeLeftOver;

		while (timeToSpend > timeBetweenParticles)
		{
			currentTime += timeBetweenParticles;
			timeToSpend -= timeBetweenParticles;

			Vector3 pos = Vector3(
				RandomFloat(settings.min_position.x, settings.max_position.x),
				RandomFloat(settings.min_position.y, settings.max_position.y),
				RandomFloat(settings.min_position.z, settings.max_position.z));
			AddParticle(pos);
		}

		timeLeftOver = timeToSpend;
	}

	RetireActiveParticles();
	FreeRetiredParticles();

	if (first_active_particle == first_free_particle)
		current_time = 0.0f;

	if (first_retired_particle == first_active_particle)
		draw_counter = 0.0f;

	particle_shader->Update(Matrix::Identity, viewMatrix, projectionMatrix);
}

void ParticleSystem::Draw(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
{
	if (first_new_particle != first_free_particle)
	{
		AddNewParticlesToVertexBuffer(deviceContext);
	}

	if (first_active_particle != first_free_particle)
	{
		if (settings.blend_state == BLEND_STATE::Alpha)
		{
			deviceContext->OMSetBlendState(StatesHelper::GetInstance().GetStates()->NonPremultiplied(), Black, 0xFFFFFFFF);
		}
		else if (settings.blend_state == BLEND_STATE::Additive)
		{
			deviceContext->OMSetBlendState(StatesHelper::GetInstance().GetStates()->Additive(), Black, 0xFFFFFFFF);
		}
			
		
		deviceContext->OMSetDepthStencilState(StatesHelper::GetInstance().GetStates()->DepthRead(), 0);

		unsigned int stride = sizeof(ParticleVertex);
		unsigned int offset = 0;

		deviceContext->IASetVertexBuffers(0, 1, vertex_buffer->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		deviceContext->IASetIndexBuffer(this->vertex_buffer->GetIndexBuffer().Get() , DXGI_FORMAT_R32_UINT, 0);
		deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		if (first_active_particle < first_free_particle)
		{
			particle_shader.get()->Draw(deviceContext, (first_free_particle - first_active_particle) * 6,
				first_active_particle * 6, 0, texture.get(), Vector2::Zero, current_time);
		}
		else
		{
			particle_shader.get()->Draw(deviceContext, (settings.max_particles - first_active_particle) * 6,
				first_active_particle * 6, 0, texture.get(), Vector2::Zero, current_time);

			if (first_free_particle > 0)
			{
				particle_shader.get()->Draw(deviceContext, first_free_particle * 6, 0, 0, texture.get(),
					Vector2::Zero, current_time, false);
			}
		}
	}

	draw_counter++;
}

void ParticleSystem::Initialize()
{
	particles = shared_ptr<ParticleVertex>(new ParticleVertex[settings.max_particles * 4]);
	auto p_particles = particles.get();

	// make quads
	for (int i = 0; i < settings.max_particles; ++i)
	{
		p_particles[i * 4 + 0].Corner = Vector2(-1.0f, -1.0f); // bottom left
		p_particles[i * 4 + 1].Corner = Vector2(1.0f, -1.0f);  // bottom right
		p_particles[i * 4 + 2].Corner = Vector2(1.0f, 1.0f);   // top right
		p_particles[i * 4 + 3].Corner = Vector2(-1.0f, 1.0f);  // top left
	}
}

void ParticleSystem::LoadContent(ComPtr<ID3D11Device> device)
{
	LoadParticleEffect();

	vector<unsigned long> indices;

	for (int i = 0; i < settings.max_particles; ++i)
	{
		indices.push_back(i * 4 + 0);
		indices.push_back(i * 4 + 2);
		indices.push_back(i * 4 + 1);

		indices.push_back(i * 4 + 0);
		indices.push_back(i * 4 + 3);
		indices.push_back(i * 4 + 2);
	}

	// create a dynamic vertex buffer
	vertex_buffer = VertexBuffer::CreateDynamic(device, sizeof(ParticleVertex),
		settings.max_particles * 4, indices, indices.size());
}

void ParticleSystem::LoadParticleEffect()
{
}

void ParticleSystem::RetireActiveParticles()
{
	float particle_duration = settings.duration;

	while (first_active_particle != first_new_particle)
	{
		float particle_age = current_time - particles.get()[first_active_particle * 4].Time;

		if (particle_age < particle_duration)
			break;

		particles.get()[first_active_particle * 4].Time = draw_counter;

		first_active_particle++;

		if (first_active_particle >= settings.max_particles)
			first_active_particle = 0;
	}
}

void ParticleSystem::FreeRetiredParticles()
{
	while (first_retired_particle != first_active_particle)
	{
		int age = draw_counter - (int)particles.get()[first_retired_particle * 4].Time;

		if (age < 3)
			break;

		first_retired_particle++;

		if (first_retired_particle >= settings.max_particles)
			first_retired_particle = 0;
	}
}

void ParticleSystem::AddNewParticlesToVertexBuffer(ComPtr<ID3D11DeviceContext> deviceContext)
{
	int stride = ParticleVertex::GetSize();

	if (first_new_particle < first_free_particle)
	{
		// all new particles are in one consecutive range
		// can upload with a single write
		vertex_buffer.get()->SetData<ParticleVertex>(deviceContext, particles.get(),
			first_new_particle * 4, (first_free_particle - first_new_particle) * 4);
	}
	else
	{
		// our new particle range wraps past the end of the array, need to do 2 writes
		vertex_buffer.get()->SetData<ParticleVertex>(deviceContext, particles.get(),
			first_new_particle * 4, (settings.max_particles - first_new_particle) * 4);

		if (first_free_particle > 0)
		{
			vertex_buffer.get()->SetData<ParticleVertex>(deviceContext, particles.get(),
				0, first_free_particle * 4);
		}
	}

	first_new_particle = first_free_particle;
}

void ParticleSystem::AddParticle(Vector3 position)
{
	int next_free_particle = first_free_particle + 1;

	if (next_free_particle >= settings.max_particles)
		next_free_particle = 0;

	if (next_free_particle == first_retired_particle)
		return;

	Vector3 velocity = Vector3
	(
		RandomFloat(settings.min_velocity.x, settings.max_velocity.x),
		RandomFloat(settings.min_velocity.y, settings.max_velocity.y),
		RandomFloat(settings.min_velocity.z, settings.max_velocity.z)
	);

	Color random = Color(RandomFloat(0.0f, 1.0f), RandomFloat(0.0f, 1.0f), 
		RandomFloat(0.0f, 1.0f), RandomFloat(0.0f, 1.0f));

	for (int i = 0; i < 4; i++)
	{
		particles.get()[first_free_particle * 4 + i].Position = position;
		particles.get()[first_free_particle * 4 + i].Velocity = velocity;
		particles.get()[first_free_particle * 4 + i].Random = random;
		particles.get()[first_free_particle * 4 + i].Time = current_time;
	}

	first_free_particle = next_free_particle;
}
