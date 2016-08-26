#pragma once

#include <iostream>

#include "SimpleMath.h"

using namespace std;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;


/* A transform is a simple data structure that describes
the Translation, Rotation and Scale of an object in 3D space */
struct Transform
{
public:
	// position in 3d space (x,y,z)
	Vector3 _position;

	// Rotation on axis' (x,y,z) in radians
	Vector3 _rotation;

	// Scale on each axis (x,y,z) in float scalar
	Vector3 _scale;

	Transform() 
	{
		_position = Vector3::Zero, _rotation = Vector3::Zero, _scale = Vector3(1.0f, 1.0f, 1.0f); ComputeWorldMatrix(); 
	}

	Transform(Vector3 pos, Vector3 rot, Vector3 scale)
	{
		_position = pos;
		_rotation = rot;
		_scale = scale;
		ComputeWorldMatrix();
	}

	void Translate(float dx, float dy, float dz)
	{
		_position.x += dx;
		_position.y += dy;
		_position.z += dz;
		ComputeWorldMatrix();

	}

	void Translate(Vector3 v)
	{
		_position += v;
		ComputeWorldMatrix();
	}

	void Rotate(float dx, float dy, float dz)
	{
		_rotation.x += dx;
		_rotation.y += dy;
		_rotation.z += dz;
		ComputeWorldMatrix();
	}

	void Rotate(Vector3 v)
	{
		_rotation += v;
		ComputeWorldMatrix();
	}

	void Scale(float dx, float dy, float dz)
	{
		_scale.x += dx;
		_scale.y += dy;
		_scale.z += dz;
		ComputeWorldMatrix();
	}

	void Scale(Vector3 v)
	{
		_scale += v;
		ComputeWorldMatrix();
	}

	Matrix GetWorldMatrix(bool recompute = false)
	{
		if (recompute)
			ComputeWorldMatrix();
		return _worldMatrix;
	}

private:
	// pre-computed world matrix, updated each time Position, Scale or Rotation is updated
	Matrix _worldMatrix;

	// builds a world Matrix that describes this Transforms
	// attributes.
	void ComputeWorldMatrix()
	{
		_worldMatrix = Matrix::CreateScale(_scale) * Matrix::CreateRotationX(_rotation.x) * Matrix::CreateRotationY(_rotation.y) * Matrix::CreateRotationZ(_rotation.z) * Matrix::CreateTranslation(_position);
	}
};