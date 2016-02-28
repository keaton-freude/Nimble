#include "ParticleSettings.h"
#include "Logger.h"

ParticleSettings::ParticleSettings()
	: texture_name("..\\..\\Assets\\Textures\\smoke_particle.tga"), max_particles(2000000),
	duration(12.0f), duration_randomness(3.0f), min_velocity(Vector3(-5.0f, -5.0f, -5.0f)), max_velocity(Vector3(5.0f, 5.0f, 5.0f)),
	gravity(Vector3(0.0f, 0.0f, 0.0f)), end_velocity(1.0f), min_color(Color(1.0f, 1.0f, 1.0f, 1.0f)), max_color(Color(1.0f, 1.0f, 1.0f, 1.0f)),
	min_rotate_speed(0.0f), max_rotate_speed(0.0f), start_size(0.0f, 0.0f), end_size(0.0f, 0.0f),
	blend_state(BLEND_STATE::Alpha)
{
	LOG_INFO("ParticleSettings default c'tor");
}

