#pragma once
#include "Typedefs.h"
#include "Logger.h"


using std::string;

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

	float particles_per_second;

	string system_name;

	// how long each particle will last in seconds
	// between duration.x and duration.y
	float duration;

	// if greater than zero, some particles will last a shorter time than others
	float duration_randomness;

	// Create a random velocity vector constrained between the two vectors below
	// makes each particle has different velocities
	Vector3 min_velocity;
	Vector3 max_velocity;
	Vector3 min_position;
	Vector3 max_position;
	Vector3 min_acceleration;
	Vector3 max_acceleration;

	Vector3 gravity;

	// interpolates from 1 -> end_velocity
	// if set to 1, particles end velocity is same as start
	// if set to 0, particles end velocity is 0
	float end_velocity;

	Color min_color;
	Color max_color;


	float min_rotate_speed;
	float max_rotate_speed;

	// actual value chosen randomly between start_size.x and start_size.y
	Vector2 start_size;
	Vector2 end_size;

	// defines the scaling of the particle quad
	// a narrow verticle bar might be: x: .1, y: 1.0
	// each component must be between 0 and 1.0 and describes
	// how wide/tall that dimension is. .10 = 10%, .5 = 50%, etc.
	Vector2 geometry_scale;

	BLEND_STATE blend_state;

	ParticleSettings();

	explicit ParticleSettings(string file_path);

	void WriteToFile(string file_path) const;
};