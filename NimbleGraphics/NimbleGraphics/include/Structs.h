#pragma once

#include <d3d11.h>
#include <SimpleMath.h>

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

typedef struct HeightMapData
{
	float x, y, z;
	// normals
	float tu, tv;
	float nx, ny, nz;
} HeightMapData;

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

typedef struct TerrainVertex
{
	Vector3 position;
	Vector2 texture;
	Vector3 normal;
} TerrainVertex;

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