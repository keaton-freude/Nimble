#pragma once
#include <SimpleMath.h>

class Emitter
{
public:
	Emitter();
	~Emitter();

private:
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Vector3 direction;
	int fire_rate; // per update, how many particles to emit
};
