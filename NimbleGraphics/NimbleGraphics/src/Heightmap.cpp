#include "Heightmap.h"
#include "Logger.h"
#include "Helper.h"
#include <stdio.h>

using namespace std;

using DirectX::XM_PIDIV2;

Heightmap::Heightmap()
	: highest_point(0), _splineX({ 0, 0.124286, 0.325714, 0.492857, 0.721429, 0.94, 1.0 }),
	_splineY({ 0, 0.20743, 0.403509, 0.691434, 0.915377, 0.93808, 1.0 }),
	_spline()
{
	_image = bitmap_image();
	_spline.set_points(_splineX, _splineY);
}

Heightmap::~Heightmap()
{
	LOG_INFO("Heightmap destruct!");
}


Heightmap::Heightmap(unsigned int width, unsigned int height, float resolution)
	: _splineX({ 0, 0.05, 0.0742857, 0.0985714, 0.12, 0.142857, 0.171429, 0.18, 0.198571, 0.218571, 0.24, 0.295714, 0.377143, 0.498571, 0.617143, 0.75, 0.875714, 0.977143, 1.0 }),
	_splineY({ 0, 0.0330237, 0.0784314, 0.150671, 0.21775, 0.335397, 0.44066, 0.540764, 0.664603, 0.778122, 0.858617, 0.909185, 0.951496, 0.973168, 0.986584, 1.0, 1.0, 1.0, 1.0 }),
	_image(width, height), _spline()
{
	_spline.set_points(_splineX, _splineY);

	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;

	_image.set_all_channels(0, 0, 0);

	_heightMap = shared_ptr<TerrainVertex>(new TerrainVertex[_image.width() * _image.height()]);

	for (unsigned int j = 0; j < _image.height(); j++)
	{
		for (unsigned int i = 0; i < _image.width(); i++)
		{
			_image.get_pixel(i, j, r, g, b);

			float vertex_height = static_cast<float>(r + g + b);

			unsigned int index = (_image.height() * j) + i;

			_heightMap.get()[index].position.x = static_cast<float>(i) * resolution;
			_heightMap.get()[index].position.y = vertex_height;
			_heightMap.get()[index].position.z = static_cast<float>(j) * resolution;
		}
	}

	for (unsigned int j = 0; j < _image.height(); j++)
	{
		for (unsigned int i = 0; i < _image.width(); i++)
		{
			_heightMap.get()[(_image.height() * j) + i].position.y /= 15.0f;
		}
	}

	this->CalculateNormals();
	this->CalculateHighestPoint();
}

Heightmap::Heightmap(ComPtr<ID3D11Device> device, string heightMapPath)
{
	this->LoadFromFile(device, heightMapPath);
}

// min = value on the far outside of the circle
// max = value on the very center of the circle
// both in terms of intensity
void Heightmap::Add(Vector3 location, float radius, float min, float max)
{
	unsigned int height = _image.height();
	unsigned int width = _image.width();
	unsigned char r;
	unsigned char g;
	unsigned char b;

	for (unsigned int i = 0; i < width; ++i)
	{
		for (unsigned int j = 0; j < height; ++j)
		{
			unsigned int index = i * width + j;
			float distance = Vector3::Distance(location, _heightMap.get()[index].position);
			// Check distance of this vertex with passed in location
			if (distance <= radius)
			{
				float lerp_amount = (distance / radius);
				if (lerp_amount > 1.0f)
				{
					lerp_amount = 1.0f;
				}

				// need to increase the values here. will lerp between 0 and 10 for now
				float lerp_value = lerp(1.0f, 0.0f, lerp_amount);

				float height_increase;

				if (lerp_amount < .3f)
					height_increase = static_cast<float>(_spline(lerp_value)) * 300.0f;
				else
					height_increase = 300.0f;



				_image.get_pixel(i, j, r, g, b);

				float split_increase = height_increase / 3.0f;


				if (split_increase >= r)
				{
					if (r + split_increase > 255)
						r = 255;
					else
						r = static_cast<unsigned char>(split_increase);
				}

				if (split_increase >= g)
				{
					if (g + split_increase > 255)
						g = 255;
					else
						g = static_cast<unsigned char>(split_increase);
				}

				if (split_increase >= b)
				{
					if (b + split_increase > 255)
						b = 255;
					else
						b = static_cast<unsigned char>(split_increase);
				}

				_image.set_pixel(i, j, r, g, b);

				// set the new value in our heightmap array
				_heightMap.get()[index].position.y = (r + g + b) / 35.0f;
			}
		}
	}

	this->CalculateNormals();
}

void Heightmap::SmoothAdd(Vector3 location, float radius, float intensity)
{
	const auto width = _image.width();
	const auto height = _image.height();
	auto currentMapPosition = Vector3::Zero;
	const auto p_heightmap = _heightMap.get();
	unsigned char r;
	unsigned char g;
	unsigned char b;
	auto const dampening_factor = 20.0f;

	for(auto i = 0; i < width; ++i)
	{
		for(auto j = 0; j < height; ++j)
		{
			auto index = i * width + j;
			currentMapPosition = p_heightmap[index].position;

			// Find the distance from the current point from the given location
			auto distance = Vector3::Distance(location, currentMapPosition);

			// If this point is within the radius, we need to transform it
			if (distance <= radius)
			{
				// the point in the middle will have the highest
				// value, while the points on the very edge will be
				// zero

				// distance_factor will be 1.0 when exactly on the location, 0.0 when exactly
				// on the edge of radius
				auto distance_factor = 1 - (distance / radius);

				auto amount = lerp(0.0, intensity, distance_factor);

				_image.get_pixel(i, j, r, g, b);

				auto split_increase = amount / 3.0f;

				r = overflow_safe_add(r, split_increase, 0, 255);
				g = overflow_safe_add(r, split_increase, 0, 255);
				b = overflow_safe_add(r, split_increase, 0, 255);

				_image.set_pixel(i, j, r, g, b);

				auto heightmap_value = (r + g + g) / dampening_factor;

				if (heightmap_value > highest_point)
					highest_point = heightmap_value;

				// set the new value in our heightmap array
				_heightMap.get()[index].position.y = heightmap_value;

				
			}
		}
	}

	this->CalculateNormals();
}

void Heightmap::Save()
{
	_image.save_image("testHeightmap.bmp");
}

void Heightmap::LoadFromFile(ComPtr<ID3D11Device> device, string heightMapPath)
{
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;

	_image = bitmap_image(heightMapPath);
	unsigned int height = _image.height();
	unsigned int width = _image.width();

	_heightMap = shared_ptr<TerrainVertex>(new TerrainVertex[_image.width() * _image.height()]);

	for (unsigned int j = 0; j < height; j++)
	{
		for (unsigned int i = 0; i < width; i++)
		{
			_image.get_pixel(i, j, r, g, b);

			float vertex_height = static_cast<float>(r + g + b);

			unsigned int index = (_image.height() * j) + i;

			_heightMap.get()[index].position.x = static_cast<float>(i);
			_heightMap.get()[index].position.y = vertex_height;
			_heightMap.get()[index].position.z = static_cast<float>(j);
		}
	}

	// normalize heightmap (should be separate functionality)

	for (unsigned int j = 0; j < _image.height(); j++)
	{
		for (unsigned int i = 0; i < _image.width(); i++)
		{
			_heightMap.get()[(_image.height()* j) + i].position.y /= 15.0f;
		}
	}

	this->CalculateNormals();
}

bool Heightmap::CalculateNormals()
{
	int index1, index2, index3, index, count;
	float vertex1[3], vertex2[3], vertex3[3], vector1[3], vector2[3], sum[3], length;
	unique_ptr<VectorType> normals;

	unsigned int height = _image.height();
	unsigned int width = _image.width();


	// Create a temporary array to hold the un-normalized normal vectors.
	normals = unique_ptr<VectorType>(new VectorType[(height - 1) * (width - 1)]);

	// Go through all the faces in the mesh and calculate their normals.
	for (int j = 0; j < (int)(height - 1); j++)
	{
		for (int i = 0; i < (int)(width - 1); i++)
		{
			index1 = (j * height) + i;
			index2 = (j * height) + (i + 1);
			index3 = ((j + 1) * height) + i;

			// Get three vertices from the face.
			vertex1[0] = _heightMap.get()[index1].position.x;
			vertex1[1] = _heightMap.get()[index1].position.y;
			vertex1[2] = _heightMap.get()[index1].position.z;

			vertex2[0] = _heightMap.get()[index2].position.x;
			vertex2[1] = _heightMap.get()[index2].position.y;
			vertex2[2] = _heightMap.get()[index2].position.z;

			vertex3[0] = _heightMap.get()[index3].position.x;
			vertex3[1] = _heightMap.get()[index3].position.y;
			vertex3[2] = _heightMap.get()[index3].position.z;

			// Calculate the two vectors for this face.
			vector1[0] = vertex1[0] - vertex3[0];
			vector1[1] = vertex1[1] - vertex3[1];
			vector1[2] = vertex1[2] - vertex3[2];
			vector2[0] = vertex3[0] - vertex2[0];
			vector2[1] = vertex3[1] - vertex2[1];
			vector2[2] = vertex3[2] - vertex2[2];

			index = (j * (height - 1)) + i;

			// Calculate the cross product of those two vectors to get the un-normalized value for this face normal.
			normals.get()[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
			normals.get()[index].y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
			normals.get()[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);
		}
	}

	// Now go through all the vertices and take an average of each face normal 	
	// that the vertex touches to get the averaged normal for that vertex.
	for (int j = 0; j < (int)height; j++)
	{
		for (int i = 0; i < (int)width; i++)
		{
			// Initialize the sum.
			sum[0] = 0.0f;
			sum[1] = 0.0f;
			sum[2] = 0.0f;

			// Initialize the count.
			count = 0;

			// Bottom left face.
			if (((i - 1) >= 0) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (height - 1)) + (i - 1);

				sum[0] += normals.get()[index].x;
				sum[1] += normals.get()[index].y;
				sum[2] += normals.get()[index].z;
				count++;
			}

			// Bottom right face.
			if ((i < (int)(width - 1)) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (height - 1)) + i;

				sum[0] += normals.get()[index].x;
				sum[1] += normals.get()[index].y;
				sum[2] += normals.get()[index].z;
				count++;
			}

			// Upper left face.
			if (((i - 1) >= 0) && (j < (int)(height - 1)))
			{
				index = (j * (height - 1)) + (i - 1);

				sum[0] += normals.get()[index].x;
				sum[1] += normals.get()[index].y;
				sum[2] += normals.get()[index].z;
				count++;
			}

			// Upper right face.
			if ((i < (int)(width - 1)) && (j < (int)(height - 1)))
			{
				index = (j * (height - 1)) + i;

				sum[0] += normals.get()[index].x;
				sum[1] += normals.get()[index].y;
				sum[2] += normals.get()[index].z;
				count++;
			}

			// Take the average of the faces touching this vertex.
			sum[0] = (sum[0] / (float)count);
			sum[1] = (sum[1] / (float)count);
			sum[2] = (sum[2] / (float)count);

			// Calculate the length of this normal.
			length = sqrt((sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));

			// Get an index to the vertex location in the height map array.
			index = (j * height) + i;

			// Normalize the final shared normal for this vertex and store it in the height map array.
			_heightMap.get()[index].normal.x = (sum[0] / length);
			_heightMap.get()[index].normal.y = (sum[1] / length);
			_heightMap.get()[index].normal.z = (sum[2] / length);
		}
	}

	return true;
}

void Heightmap::CalculateHighestPoint()
{
	const auto width = _image.width();
	const auto height = _image.height();

	for(auto i = 0; i < width; ++i)
	{
		for(auto j = 0; j < height; ++j)
		{
			auto index = i * width + j;

			auto location_height = _heightMap.get()[index].position.y;

			if (location_height > this->highest_point)
				this->highest_point = location_height;
		}
	}
}

shared_ptr<TerrainVertex> Heightmap::GetHeightMapData()
{
	return _heightMap;
}

unsigned int Heightmap::GetWidth()
{
	return _image.width();
}

unsigned int Heightmap::GetHeight()
{
	return _image.height();
}