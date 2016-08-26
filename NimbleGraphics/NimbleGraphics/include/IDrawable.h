#pragma once
#include <memory>
#include <wrl/client.h>
#include "SimpleMath.h"
#include "Transform.h"
#include "VertexBuffer.h"

using DirectX::SimpleMath::Matrix;
using std::shared_ptr;
using Microsoft::WRL::ComPtr;


class IDrawable
{
public:
	void SetVertexBuffer(shared_ptr<VertexBuffer> newBuffer)
	{
		this->vertexBuffer = newBuffer;
	}

	shared_ptr<VertexBuffer> GetVertexBuffer()
	{
		return vertexBuffer;
	}
public:
	Transform _transform;

protected:
	shared_ptr<VertexBuffer> vertexBuffer;
};