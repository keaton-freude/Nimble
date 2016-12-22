#include "D3D11Cache.h"

D3D11Cache::D3D11Cache()
{
}


void D3D11Cache::Load(D3DDevice device, D3DDeviceContext deviceContext)
{
	m_device = device;
	m_deviceContext = deviceContext;
}

D3DDevice D3D11Cache::GetDevice() const
{
	return m_device;
}

D3DDeviceContext D3D11Cache::GetDeviceContext() const
{
	return m_deviceContext;
}
