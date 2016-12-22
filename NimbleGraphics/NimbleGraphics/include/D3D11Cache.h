#pragma once
#include "Typedefs.h"
#include "Singleton.h"

// Caches a D3DDevice and D3DDeviceContext

class D3D11Cache: public Singleton<D3D11Cache>
{
public:
	void Load(D3DDevice device, D3DDeviceContext deviceContext);

	D3DDevice GetDevice() const;
	D3DDeviceContext GetDeviceContext() const;

	D3D11Cache();

private:
	D3DDevice m_device;
	D3DDeviceContext m_deviceContext;
};