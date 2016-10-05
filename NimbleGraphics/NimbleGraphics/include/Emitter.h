#pragma once
#include <d3d11.h>
#include <SimpleMath.h>

class Emitter
{
public:
	Emitter(): fire_rate(0)
	{
	}

	~Emitter()
	{
		
	}

private:
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Vector3 direction;
	int fire_rate; // per update, how many particles to emit
};
