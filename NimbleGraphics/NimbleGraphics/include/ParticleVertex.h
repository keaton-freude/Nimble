#pragma once
#include <SimpleMath.h>
#include <memory>

using std::shared_ptr;
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Color;

struct ParticleVertex
{
	Vector2 Corner; 
	Vector3 Position;
	Vector3 Velocity;
	Color Random;
	float Time; 

	static size_t GetSize() 
	{
		return sizeof(ParticleVertex);
	}

	static shared_ptr<D3D11_INPUT_ELEMENT_DESC> GetLayout(unsigned int & numElements)
	{
		numElements = 5;

		auto desc = shared_ptr<D3D11_INPUT_ELEMENT_DESC>(new D3D11_INPUT_ELEMENT_DESC[numElements]);

		// Corner
		desc.get()[0].SemanticName = "POSITION";
		desc.get()[0].SemanticIndex = 0;
		desc.get()[0].Format = DXGI_FORMAT_R32G32_FLOAT;
		desc.get()[0].InputSlot = 0;
		desc.get()[0].AlignedByteOffset = 0;
		desc.get()[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.get()[0].InstanceDataStepRate = 0;

		// Position
		desc.get()[1].SemanticName = "POSITION";
		desc.get()[1].SemanticIndex = 1;
		desc.get()[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		desc.get()[1].InputSlot = 0;
		desc.get()[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		desc.get()[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.get()[1].InstanceDataStepRate = 0;

		// Velocity
		desc.get()[2].SemanticName = "POSITION";
		desc.get()[2].SemanticIndex = 2;
		desc.get()[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		desc.get()[2].InputSlot = 0;
		desc.get()[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		desc.get()[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.get()[2].InstanceDataStepRate = 0;

		// Random
		desc.get()[3].SemanticName = "COLOR";
		desc.get()[3].SemanticIndex = 0;
		desc.get()[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.get()[3].InputSlot = 0;
		desc.get()[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		desc.get()[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.get()[3].InstanceDataStepRate = 0;

		// Time
		desc.get()[4].SemanticName = "TEXCOORD";
		desc.get()[4].SemanticIndex = 0;
		desc.get()[4].Format = DXGI_FORMAT_R32_FLOAT;
		desc.get()[4].InputSlot = 0;
		desc.get()[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		desc.get()[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.get()[4].InstanceDataStepRate = 0;

		return desc;
	}
};