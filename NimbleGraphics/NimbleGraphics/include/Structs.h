#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include "Logger.h"
#include <fstream>

using DirectX::SimpleMath::Color;
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Matrix;

typedef struct MeshData
{
	UINT numVertices;
	UINT numIndicies;
} MeshData;

/* necessary information for ColorShader. Contains Vec3 position and a Color vector*/
struct ColorVertex
{
	Vector3 position;
	Color color;
};


typedef struct VectorType
{
	float x, y, z;
} VectorType;

typedef struct LightBuffer
{
	Vector4 ambientLight;
	Vector4 diffuseColor;
	Vector3 lightDirection;
	float padding;
} LightBuffer;

struct TerrainVertex
{
	Vector3 position;
	Vector2 texture;
	Vector3 normal;

	TerrainVertex()
		: position(Vector3::Zero), texture(Vector2::Zero), normal(Vector3::Zero)
	{
		
	}

	TerrainVertex(TerrainVertex&& other)
		: position(std::move(other.position)), texture(std::move(other.texture)), normal(std::move(other.normal))
	{
	}

	TerrainVertex(Vector3 pos, Vector2 tex, Vector3 norm)
		: position(pos), texture(tex), normal(norm)
	{
		
	}

	TerrainVertex& operator=(TerrainVertex& other)
	{
		this->position = other.position;
		this->normal = other.normal;
		this->texture = other.texture;
		return *this;
	}

	TerrainVertex& operator=(TerrainVertex&& other) // move assignment
	{
		this->position = std::move(other.position);
		this->normal = std::move(other.normal);
		this->texture = std::move(other.texture);
		return *this;
	}

	void WriteToFile(std::fstream & file)
	{
		//file << "wat?";
		//file << std::string("wat").c_str();
		file << position.x << "," << position.y << "," << position.z << "," << texture.x << "," << texture.y << "," << normal.x << "," << normal.y << "," << normal.z;
	}

};

const int TEXTURE_REPEAT = 2;

typedef struct TargaHeader
{
	unsigned char data1[12];
	unsigned short width;
	unsigned short height;
	unsigned char bpp;
	unsigned char data2;
} TargaHeader;

_declspec(align(16))
struct TimeBuffer
{
	float TimeStarted;
	float CurrentTime;
};

// Need to make sure we align our data as the shader expects
// which is in 16 byte boundaries (can't let our values slip
// between two boundaries).
_declspec(align(16))
struct ConstantParticleParameters
{
	Vector3 Gravity;
	float Duration; // 16 bytes, new boundary

	Color MinColor; // 16 bytes, new boundary

	Color MaxColor; // 16 bytes, new boundary

	Vector2 RotateSpeed;
	float DurationRandomness;
	float EndVelocity; // 16 bytes, new boundary
	Vector2 StartSize;
	Vector2 EndSize; // 16 bytes, new boundary
};

_declspec(align(16))
struct PerFrameParticleParameters
{
	Vector2 ViewportScale;
	float CurrentTime;
};

struct TerrainCell
{
	TerrainVertex bottomLeft;
	TerrainVertex bottomRight;
	TerrainVertex upperLeft;
	TerrainVertex upperRight;

	Vector3 GetAveragePosition()
	{
		// average all 4 vertices and return
		Vector3 average = bottomLeft.position + bottomRight.position + upperLeft.position + upperRight.position;

		return average * 0.25f;
	}
};