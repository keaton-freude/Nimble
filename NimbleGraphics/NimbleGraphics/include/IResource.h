#pragma once
#include <d3d11.h>
#include <wrl\client.h>
using Microsoft::WRL::ComPtr;


class IResource
{
public:
	IResource();
	IResource(IResource&& other);
	IResource& operator=(IResource&& other);
	virtual bool Load(ComPtr<ID3D11Device> device) = 0;
	virtual void Unload() = 0;
private:
};