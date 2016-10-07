#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, const ParticleSettings& settings): settings(settings), system_time(0.0f), timeLeftOver(0.0f), first_active_particle(0), first_new_particle(0),
                                                                                                                                          first_free_particle(0), first_retired_particle(0), startIndex(0), length(0), timeBetweenParticles(1.0f / settings.particles_per_second)
{
	// each particle system needs it own particle shader because we rely on
	// constant shader values, and we can't stomp over another particle system's
	// cbuffer space (alternative is to re-write these values per frame, instead of per
	// lifetime of the system).
	particle_shader.reset(new ParticleShader(device, settings));

	texture = std::make_unique<Texture>(device, deviceContext, settings.texture_name);

	Initialize();
	LoadContent(device);
}

void ParticleSystem::Update(const Matrix& viewMatrix, const Matrix& projectionMatrix, float dt)
{
	system_time += dt;

	if (dt > 0)
	{
		float timeToSpend = timeLeftOver + dt;

		float currentTime = -timeLeftOver;

		while (timeToSpend > timeBetweenParticles)
		{
			currentTime += timeBetweenParticles;
			timeToSpend -= timeBetweenParticles;

			auto pos = RandomVector3(settings.min_position, settings.max_position);

			AddParticle(pos);
		}

		timeLeftOver = timeToSpend;
	}

	RetireActiveParticles();
	FreeRetiredParticles();

	if (first_active_particle == first_free_particle)
		system_time = 0.0f;

	if (first_retired_particle == first_active_particle)
		draw_counter = 0;

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
			deviceContext->OMSetBlendState(StatesHelper::GetInstance().GetStates()->NonPremultiplied(), DirectX::Colors::Black, 0xFFFFFFFF);
		}
		else if (settings.blend_state == BLEND_STATE::Additive)
		{
			deviceContext->OMSetBlendState(StatesHelper::GetInstance().GetStates()->Additive(), DirectX::Colors::Black, 0xFFFFFFFF);
		}


		deviceContext->OMSetDepthStencilState(StatesHelper::GetInstance().GetStates()->DepthRead(), 0);

		unsigned int stride = sizeof(ParticleVertex);
		unsigned int offset = 0;

		deviceContext->IASetVertexBuffers(0, 1, vertex_buffer->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		deviceContext->IASetIndexBuffer(this->vertex_buffer->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
		deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		if (first_active_particle < first_free_particle)
		{
			particle_shader.get()->Draw(deviceContext, (first_free_particle - first_active_particle) * 6,
			                            first_active_particle * 6, 0, texture.get(), Vector2::Zero, system_time);
		}
		else
		{
			particle_shader.get()->Draw(deviceContext, (settings.max_particles - first_active_particle) * 6,
			                            first_active_particle * 6, 0, texture.get(), Vector2::Zero, system_time);

			if (first_free_particle > 0)
			{
				particle_shader.get()->Draw(deviceContext, first_free_particle * 6, 0, 0, texture.get(),
				                            Vector2::Zero, system_time, false);
			}
		}
	}

	draw_counter++;
}

void ParticleSystem::Initialize()
{
	particles = std::make_unique<ParticleVertex[]>(settings.max_particles * 4);
	auto p_particles = particles.get();

	auto x_mul = settings.geometry_scale.x;
	auto y_mul = settings.geometry_scale.y;

	// make quads
	for (auto i = 0; i < settings.max_particles; ++i)
	{
		p_particles[i * 4 + 0].Corner = Vector2(-x_mul, -y_mul); // bottom left
		p_particles[i * 4 + 1].Corner = Vector2(x_mul, -y_mul); // bottom right
		p_particles[i * 4 + 2].Corner = Vector2(x_mul, y_mul); // top right
		p_particles[i * 4 + 3].Corner = Vector2(-x_mul, y_mul); // top left
	}
}

void ParticleSystem::LoadContent(ComPtr<ID3D11Device> device)
{
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
	vertex_buffer = Mesh::CreateDynamic(device, sizeof(ParticleVertex),
	                                    settings.max_particles * 4, indices, indices.size());
}

void ParticleSystem::RetireActiveParticles()
{
	float particle_duration = settings.duration;

	while (first_active_particle != first_new_particle)
	{
		float particle_age = system_time - particles.get()[first_active_particle * 4].Time;
		particle_age *= 1 + particles.get()[first_active_particle * 4].Random.x * settings.duration_randomness;
		// apply duration_randomness


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
		int age = draw_counter - static_cast<int>(particles.get()[first_retired_particle * 4].Time);

		if (age < 3)
			break;

		first_retired_particle++;

		if (first_retired_particle >= settings.max_particles)
			first_retired_particle = 0;
	}
}

void ParticleSystem::AddNewParticlesToVertexBuffer(ComPtr<ID3D11DeviceContext> deviceContext)
{
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

	auto velocity = RandomVector3(settings.min_velocity, settings.max_velocity);

	Color random = Color(RandomFloat(0.0f, 1.0f), RandomFloat(0.0f, 1.0f),
	                     RandomFloat(0.0f, 1.0f), RandomFloat(0.0f, 1.0f));

	for (int i = 0; i < 4; i++)
	{
		particles.get()[first_free_particle * 4 + i].Position = position;
		particles.get()[first_free_particle * 4 + i].Velocity = velocity;
		particles.get()[first_free_particle * 4 + i].Random = random;
		particles.get()[first_free_particle * 4 + i].Time = system_time;
	}

	first_free_particle = next_free_particle;
}
