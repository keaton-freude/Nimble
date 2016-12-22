#pragma once
#include "Typedefs.h"
#include "D3D11Cache.h"

class D3D11Object
{
public:
	D3D11Object()
	{
		m_device = D3D11Cache::GetInstance().GetDevice();
		m_deviceContext = D3D11Cache::GetInstance().GetDeviceContext();
	}
protected:
	D3DDevice m_device;
	D3DDeviceContext m_deviceContext;
};
