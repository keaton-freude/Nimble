#pragma once
#include <string>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <SimpleMath.h>
#include <fstream>
#include "Logger.h"


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

	ParticleSettings() : texture_name("..\\..\\Assets\\Textures\\smoke_particle.tga"), max_particles(2000000),
		particles_per_second(0.0f), duration(12.0f), duration_randomness(3.0f), min_velocity(Vector3(-5.0f, -5.0f, -5.0f)),
		max_velocity(Vector3(5.0f, 5.0f, 5.0f)), gravity(Vector3(0.0f, 0.0f, 0.0f)), end_velocity(1.0f), min_color(Color(1.0f, 1.0f, 1.0f, 1.0f)),
		max_color(Color(1.0f, 1.0f, 1.0f, 1.0f)), min_rotate_speed(0.0f), max_rotate_speed(0.0f), start_size(0.0f, 0.0f),
		end_size(0.0f, 0.0f), geometry_scale(1.0f, 1.0f), blend_state(BLEND_STATE::Alpha)
	{
	}

	explicit ParticleSettings(string file_path): texture_name("..\\..\\Assets\\Textures\\smoke_particle.tga"), max_particles(2000000),
		particles_per_second(0.0f), duration(12.0f), duration_randomness(3.0f), min_velocity(Vector3(-5.0f, -5.0f, -5.0f)),
		max_velocity(Vector3(5.0f, 5.0f, 5.0f)), gravity(Vector3(0.0f, 0.0f, 0.0f)), end_velocity(1.0f), min_color(Color(1.0f, 1.0f, 1.0f, 1.0f)),
		max_color(Color(1.0f, 1.0f, 1.0f, 1.0f)), min_rotate_speed(0.0f), max_rotate_speed(0.0f), start_size(0.0f, 0.0f),
		end_size(0.0f, 0.0f), geometry_scale(1.0f, 1.0f), blend_state(BLEND_STATE::Alpha)
	{
		std::ifstream ifs(file_path);

		// read the whole file into a string
		string content((std::istreambuf_iterator<char>(ifs)),
			(std::istreambuf_iterator<char>()));

		// set our settings based on the data
		rapidjson::Document document;

		if (document.Parse(content.c_str()).HasParseError())
		{
			LOG_ERROR("Unable to load JSON ParticleSystem data in ", file_path);
			return;
		}

		if (document.HasMember("system_name"))
		{
			system_name = document["system_name"].GetString();
		}

		if (document.HasMember("max_particles"))
		{
			max_particles = document["max_particles"].GetUint();
		}

		if (document.HasMember("texture_name"))
		{
			texture_name = document["texture_name"].GetString();
		}

		if (document.HasMember("particles_per_second"))
		{
			particles_per_second = static_cast<float>(document["particles_per_second"].GetDouble());
		}
	}

	void WriteToFile(string file_path)
	{
		rapidjson::StringBuffer sb;
		rapidjson::Writer<rapidjson::StringBuffer> writer(sb);

		writer.StartObject();
		writer.Key("system_name");
		writer.String(system_name.c_str());
		writer.Key("max_particles");
		writer.Uint(max_particles);
		writer.Key("texture_name");
		writer.String(texture_name.c_str());
		writer.Key("particles_per_second");
		writer.Double(particles_per_second);
		writer.EndObject();

		std::ofstream out_file(file_path, std::ios::out | std::ios::trunc);

		out_file << sb.GetString();

		out_file.close();
	}
};