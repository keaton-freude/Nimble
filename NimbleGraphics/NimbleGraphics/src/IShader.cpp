#include "IShader.h"

IShader::IShader()
{
	LOG_INFO("IShader default c'tor");
}

IShader::IShader(D3DDevice device, D3DDeviceContext deviceContext)
	: _device(device), _deviceContext(deviceContext)
{
}

IShader::~IShader()
{
	LOG_INFO("IShader Destruct!");
}