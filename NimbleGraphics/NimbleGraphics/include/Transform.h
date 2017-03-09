#pragma once
#include "Typedefs.h"


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

	Transform();
	Transform(Vector3 pos, Vector3 rot, Vector3 scale);

	void Translate(float dx, float dy, float dz);
	void Translate(Vector3 v);

	void Rotate(float dx, float dy, float dz);
	void Rotate(Vector3 v);

	void Scale(float dx, float dy, float dz);
	void Scale(Vector3 v);

	Matrix GetWorldMatrix(bool recompute = false);

private:
	// pre-computed world matrix, updated each time Position, Scale or Rotation is updated
	Matrix _worldMatrix;

	// builds a world Matrix that describes this Transforms
	// attributes.
	void ComputeWorldMatrix();
};