#include "Transform.h"

Transform::Transform()
{
	_position = Vector3::Zero , _rotation = Vector3::Zero , _scale = Vector3(1.0f, 1.0f, 1.0f);
	ComputeWorldMatrix();
}

Transform::Transform(Vector3 pos, Vector3 rot, Vector3 scale)
{
	_position = pos;
	_rotation = rot;
	_scale = scale;
	ComputeWorldMatrix();
}

void Transform::Translate(float dx, float dy, float dz)
{
	_position.x += dx;
	_position.y += dy;
	_position.z += dz;
	ComputeWorldMatrix();
}

void Transform::Translate(Vector3 v)
{
	_position += v;
	ComputeWorldMatrix();
}

void Transform::Rotate(float dx, float dy, float dz)
{
	_rotation.x += dx;
	_rotation.y += dy;
	_rotation.z += dz;
	ComputeWorldMatrix();
}

void Transform::Rotate(Vector3 v)
{
	_rotation += v;
	ComputeWorldMatrix();
}

void Transform::Scale(float dx, float dy, float dz)
{
	_scale.x += dx;
	_scale.y += dy;
	_scale.z += dz;
	ComputeWorldMatrix();
}

void Transform::Scale(Vector3 v)
{
	_scale += v;
	ComputeWorldMatrix();
}

Matrix Transform::GetWorldMatrix(bool recompute)
{
	if (recompute)
		ComputeWorldMatrix();
	return _worldMatrix;
}

void Transform::ComputeWorldMatrix()
{
	_worldMatrix = Matrix::CreateScale(_scale) * Matrix::CreateRotationX(_rotation.x) * Matrix::CreateRotationY(_rotation.y) * Matrix::CreateRotationZ(_rotation.z) * Matrix::CreateTranslation(_position);
}
