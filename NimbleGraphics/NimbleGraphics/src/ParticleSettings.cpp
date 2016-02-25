#include "ParticleSettings.h"

ParticleSettings::ParticleSettings()
	: texture_name("..\\..\\Assets\\Textures\\smoke_particle.tga"), max_particles(10000),
	duration(10.0f), duration_randomness(3.0f), min_velocity(Vector3(-1.0f, 1.0f, -1.0f)), max_velocity(Vector3(1.0f, 2.0f, 1.0f)),
	gravity(Vector3(0.0f, 0.0f, 0.0f)), end_velocity(1.0f), min_color(Color(1.0f, 1.0f, 1.0f, 1.0f)), max_color(Color(1.0f, 1.0f, 1.0f, 1.0f)),
	min_rotate_speed(0.0f), max_rotate_speed(0.0f), min_start_size(0.0f), max_start_size(0.1f), min_end_size(0.9f), max_end_size(1.0f),
	blend_state(BLEND_STATE::Alpha)
{

}