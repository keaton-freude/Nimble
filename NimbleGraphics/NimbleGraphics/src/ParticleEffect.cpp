//#include "ParticleEffect.h"
//
//ParticleEffect::ParticleEffect()
//{
//}
//
//ParticleEffect::ParticleEffect(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, vector<ParticleSettings>& systems_to_create)
//{
//	for (auto const& setting : systems_to_create)
//	{
//		_systems.push_back(std::make_unique<ParticleSystem>(device, deviceContext, setting));
//	}
//}
//
//ParticleEffect::~ParticleEffect()
//{
//}
//
//void ParticleEffect::Update(const Matrix& viewMatrix, const Matrix& projectionMatrix, float dt)
//{
//	for (auto& system : _systems)
//	{
//		system->Update(viewMatrix, projectionMatrix, dt);
//	}
//}
//
//void ParticleEffect::Draw(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
//{
//	for (auto& system : _systems)
//	{
//		system->Draw(device, deviceContext);
//	}
//}
