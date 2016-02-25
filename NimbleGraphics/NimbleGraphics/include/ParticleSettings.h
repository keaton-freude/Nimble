#pragma once
#include <string>
#include <SimpleMath.h>

using std::string;
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Color;

enum BLEND_STATE
{
	Alpha = 0,
	Additive = 1,
	Opaque = 2
};

class ParticleSettings
{
public:
	// path to the texture file to use for this System
	string texture_name;

	// max number of particles
	int max_particles;

	// how long each particle will last in seconds
	float duration;

	// if greater than zero, some particles will last a shorter time than others
	float duration_randomness;

	// Create a random velocity vector constrained between the two vectors below
	// makes each particle has different velocities
	Vector3 min_velocity;
	Vector3 max_velocity;

	Vector3 gravity;

	// interpolates from 1 -> end_velocity
	// if set to 1, particles end velocity is same as start
	// if set to 0, particles end velocity is 0
	float end_velocity;

	Color min_color;
	Color max_color;

	float min_rotate_speed;
	float max_rotate_speed;

	float min_start_size;
	float max_start_size;

	float min_end_size;
	float max_end_size;

	BLEND_STATE blend_state;

	ParticleSettings();
};