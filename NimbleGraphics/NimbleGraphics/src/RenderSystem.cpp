#include "RenderSystem.h"

RenderSystem::RenderSystem()
{
}

RenderSystem::RenderSystem(D3DDevice device, D3DDeviceContext deviceContext)
	: _device(device), _deviceContext(deviceContext), _renderObjects()
{
}

void RenderSystem::Update(MatrixRef viewMatrix, MatrixRef projectionMatrix)
{
	for(auto& ro : _renderObjects)
	{
		ro.Update(viewMatrix, projectionMatrix);
	}
}

void RenderSystem::Draw()
{
	for (auto& ro : _renderObjects)
	{
		ro.Draw(_device, _deviceContext);
	}
}

RayHit RenderSystem::CastRay(const Ray& ray)
{
	for (auto& ro : _renderObjects)
	{
		auto result = ro.CastRay(ray);
		if (result.hit)
		{
			return result;
		}
	}

	return RayHit::NoHit();
}