#pragma once
#include "Typedefs.h"

class Emitter
{
public:
	Emitter();
	~Emitter();

private:
	Vector3 position;
	Vector3 direction;
	int fire_rate; // per update, how many particles to emit
};
