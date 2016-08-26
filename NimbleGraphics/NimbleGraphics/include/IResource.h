#pragma once
#include <d3d11.h>
#include <wrl\client.h>
using Microsoft::WRL::ComPtr;

class IResource
{
public:
	virtual ~IResource()
	{
	}

	IResource()
	{
		
	}

	virtual bool Load(ComPtr<ID3D11Device> device) = 0;
	virtual void Unload() = 0;
private:
};