#pragma once
#include "ShaderComponentManager.h"
#include "Logger.h"

ShaderComponentManager::ShaderComponentManager()
{
}

void ShaderComponentManager::Add(shared_ptr<IShaderComponent> component)
{
	_components.push_back(component);
}

void ShaderComponentManager::Load(D3DDevice device, D3DDeviceContext deviceContext)
{
	for (auto& component : _components)
	{
		if (!component->Load(device, deviceContext))
		{
			LOG_ERROR("Component failed to load!");
		}
	}
}

void ShaderComponentManager::Apply(D3DDevice device, D3DDeviceContext deviceContext)
{
	for (auto& component : _components)
	{
		if (!component->Apply(device, deviceContext))
		{
			LOG_ERROR("Failed to apply component!");
		}
	}
}