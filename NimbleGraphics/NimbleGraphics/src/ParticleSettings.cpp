#include "ParticleSettings.h"
#include "Logger.h"


using namespace rapidjson;
using std::ofstream;
using std::ifstream;

ParticleSettings::ParticleSettings()
	: texture_name("..\\..\\Assets\\Textures\\smoke_particle.tga"), max_particles(2000000),
	particles_per_second(0.0f), duration(12.0f), duration_randomness(3.0f), min_velocity(Vector3(-5.0f, -5.0f, -5.0f)),
	max_velocity(Vector3(5.0f, 5.0f, 5.0f)), gravity(Vector3(0.0f, 0.0f, 0.0f)), end_velocity(1.0f), min_color(Color(1.0f, 1.0f, 1.0f, 1.0f)),
	max_color(Color(1.0f, 1.0f, 1.0f, 1.0f)), min_rotate_speed(0.0f), max_rotate_speed(0.0f), start_size(0.0f, 0.0f),
	end_size(0.0f, 0.0f), blend_state(BLEND_STATE::Alpha), geometry_scale(1.0f, 1.0f)
{
	LOG_INFO("ParticleSettings default c'tor");
}

ParticleSettings::ParticleSettings(string file_path)
	: texture_name("..\\..\\Assets\\Textures\\smoke_particle.tga"), max_particles(2000000),
	particles_per_second(0.0f), duration(12.0f), duration_randomness(3.0f), min_velocity(Vector3(-5.0f, -5.0f, -5.0f)),
	max_velocity(Vector3(5.0f, 5.0f, 5.0f)), gravity(Vector3(0.0f, 0.0f, 0.0f)), end_velocity(1.0f), min_color(Color(1.0f, 1.0f, 1.0f, 1.0f)),
	max_color(Color(1.0f, 1.0f, 1.0f, 1.0f)), min_rotate_speed(0.0f), max_rotate_speed(0.0f), start_size(0.0f, 0.0f),
	end_size(0.0f, 0.0f), blend_state(BLEND_STATE::Alpha), geometry_scale(1.0f, 1.0f)
{
	ifstream ifs(file_path);
	
	// read the whole file into a string
	string content((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));

	// set our settings based on the data
	Document document;

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

void ParticleSettings::WriteToFile(string file_path)
{
	StringBuffer sb;
	Writer<StringBuffer> writer(sb);

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

	ofstream out_file(file_path, std::ios::out | std::ios::trunc);

	out_file << sb.GetString();

	out_file.close();
}
