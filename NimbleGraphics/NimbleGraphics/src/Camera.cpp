#include "Camera.h"
#include <DirectXMath.h>
#include <iostream>
using std::cout;
using std::endl;

using DirectX::XMMatrixLookAtLH;

using namespace DirectX::SimpleMath;
using DirectX::SimpleMath::Matrix;


Camera::Camera() 
	: _transform(Vector3(0.001f, 0.001f, 0.001f), 
		Vector3(0.0f, 0.0f, 0.0f), 
		Vector3(1.0f, 1.0f, 1.0f)), _viewMatrix(new Matrix())
{
}

Camera::Camera(const Camera& other)
	: _viewMatrix(new Matrix())
{
}

Camera::Camera(Vector3 initial_position)
	: _transform(initial_position,
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.0f, 0.0f, 0.0f)), _viewMatrix(new Matrix())
{
	// in case we are in an unsafe location
	this->SetSafePosition();
}

Camera::Camera(Vector3 initial_position, Vector3 initial_rotation)
	: _transform(initial_position, initial_rotation,
		Vector3(0.0f, 0.0f, 0.0f)), _viewMatrix(new Matrix())
{
	this->SetSafePosition();
}

Camera::~Camera()
{

}

void Camera::SetPosition(float x, float y, float z)
{
	_transform._position = Vector3(x, y, z);
	this->SetSafePosition();
}

void Camera::SetPosition(Vector3 new_position)
{
	_transform._position = new_position;
	this->SetSafePosition();
}

void Camera::SetSafePosition()
{
	if (_transform._position == Vector3::Zero)
	{
		// This causes divide by zero in XMMatrixLookAtLH, offset by small amount
		_transform._position += Vector3(0.001f, 0.0f, 0.001f);
	}
}

void Camera::Translate(Vector3 offset)
{
	_transform._position += offset;
	this->SetSafePosition();
}

void Camera::Translate(const float& x, const float& y, const float& z)
{
	this->Translate(Vector3(x, y, z));
}

void Camera::TranslateRotated(Vector3 offset)
{
	
}

void Camera::Rotate(Vector3 delta_rotation)
{
	_transform._rotation += delta_rotation;
}

void Camera::Update()
{
	Matrix rotationMatrixX = Matrix::CreateRotationX(_transform._rotation.x);
	Matrix rotationMatrixY = Matrix::CreateRotationY(_transform._rotation.y);

	Vector3 transformedReference = Vector3::Transform(Vector3(0.0f, 0.0f, 1.0f), rotationMatrixX);
	transformedReference = Vector3::Transform(transformedReference, rotationMatrixY);

	Vector3 cameraLookAt = _transform._position + transformedReference;

	*_viewMatrix = XMMatrixLookAtLH(_transform._position, cameraLookAt, Vector3::UnitY);
}

Ray Camera::GetMouseRay(Vector2 mousePosition, Matrix projectionMatrix)
{
	Vector3 origin;
	Vector3 final_origin;
	Vector3 final_dir;

	float pointX = mousePosition.x;
	float pointY = mousePosition.y;

	pointX = pointX / projectionMatrix._11;
	pointY = pointY / projectionMatrix._22;

	Matrix matView = *_viewMatrix;
	Matrix m = matView.Invert();

	Vector3 direction;
	direction.x = (pointX * m._11) + (pointY * m._21) + m._31;
	direction.y = (pointX * m._12) + (pointY * m._22) + m._32;
	direction.z = (pointX * m._13) + (pointY * m._23) + m._33;

	direction.Normalize();

	Vector3 rayOrig;
	rayOrig.x = m._41;
	rayOrig.y = m._42;
	rayOrig.z = m._43;

	return Ray(rayOrig, direction);
}