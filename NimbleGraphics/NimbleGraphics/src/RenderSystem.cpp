#include "RenderSystem.h"

RenderSystem::RenderSystem()
{
}

RenderSystem::RenderSystem(D3DDevice device, D3DDeviceContext deviceContext)
	: _device(device), _deviceContext(deviceContext), _renderObjects()
{
}

void RenderSystem::AddObject(const RenderObject& renderObjectToAdd)
{
	_renderObjects.push_back(renderObjectToAdd);
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
