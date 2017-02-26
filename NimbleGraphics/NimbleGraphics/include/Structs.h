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
	UINT stride;
	D3D_PRIMITIVE_TOPOLOGY topology;

	MeshData() : numVertices(0), numIndicies(0), stride(0), topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) {}
} MeshData;

/* necessary information for ColorShader. Contains Vec3 position and a Color vector*/
struct ColorVertex
{
	Vector3 position;
	Color color;
};

struct PositionVertex
{
	Vector3 position;
};

struct Triangle
{
	Triangle()
	{
		vertices[0] = Vector3::Zero;
		vertices[1] = Vector3::Zero;
		vertices[2] = Vector3::Zero;
	}

	Triangle(Vector3 one, Vector3 two, Vector3 three)
	{
		vertices[0] = one;
		vertices[1] = two;
		vertices[2] = three;
	}

	Vector3 vertices[3];
};

struct MatrixBuffer
{
	Matrix world;
	Matrix view;
	Matrix projection;
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

struct SplatBuffer
{
	int32_t numTextures;
	int32_t numSplats;
	int32_t padding[2];
};

const int TEXTURE_REPEAT = 4;

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
