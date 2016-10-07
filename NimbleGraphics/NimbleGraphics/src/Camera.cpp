#include "Camera.h"
#include "Logger.h"

Camera::Camera()
	: _transform(Vector3(0.001f, 0.001f, 0.001f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f)), 
		_viewMatrix()
{

}

Camera::Camera(Vector3 initial_position)
	: _transform(initial_position, Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f)), _viewMatrix()
{
	// in case we are in an unsafe location
	this->SetSafePosition();
}

Camera::Camera(Vector3 initial_position, Vector3 initial_rotation)
	: _transform(initial_position, initial_rotation, Vector3(0.0f, 0.0f, 0.0f)), _viewMatrix()
{
	this->SetSafePosition();
}

Camera::~Camera()
{
	LOG_INFO("Camera destruct!");
}

const Vector3& Camera::GetPosition() const
{
	return _transform._position;
}

const Vector3& Camera::GetRotation() const
{
	return _transform._rotation;
}

const Matrix& Camera::GetViewMatrix() const
{
	return _viewMatrix;
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

void Camera::Translate(Vector3 offset)
{
	_transform._position += offset;
	this->SetSafePosition();
}

void Camera::Translate(const float& x, const float& y, const float& z)
{
	this->Translate(Vector3(x, y, z));
}

void Camera::Rotate(Vector3 delta_rotation)
{
	_transform._rotation += delta_rotation;
}

void Camera::Update()
{
	auto rotationMatrixX = Matrix::CreateRotationX(_transform._rotation.x);
	auto rotationMatrixY = Matrix::CreateRotationY(_transform._rotation.y);

	auto transformedReference = Vector3::Transform(Vector3(0.0f, 0.0f, 1.0f), rotationMatrixX);
	transformedReference = Vector3::Transform(transformedReference, rotationMatrixY);

	auto cameraLookAt = _transform._position + transformedReference;

	_viewMatrix = XMMatrixLookAtLH(_transform._position, cameraLookAt, Vector3::UnitY);
}

DirectX::SimpleMath::Ray Camera::GetMouseRay(DirectX::SimpleMath::Vector2 mousePosition, Matrix projectionMatrix) const
{
	auto pointX = mousePosition.x;
	auto pointY = mousePosition.y;

	pointX = pointX / projectionMatrix._11;
	pointY = pointY / projectionMatrix._22;

	auto matView = _viewMatrix;
	auto m = matView.Invert();

	Vector3 direction;
	direction.x = (pointX * m._11) + (pointY * m._21) + m._31;
	direction.y = (pointX * m._12) + (pointY * m._22) + m._32;
	direction.z = (pointX * m._13) + (pointY * m._23) + m._33;

	direction.Normalize();

	Vector3 rayOrig;
	rayOrig.x = m._41;
	rayOrig.y = m._42;
	rayOrig.z = m._43;

	return DirectX::SimpleMath::Ray(rayOrig, direction);
}

void Camera::SetSafePosition()
{
	if (_transform._position == Vector3::Zero)
	{
		// This causes divide by zero in XMMatrixLookAtLH, offset by small amount
		_transform._position += Vector3(0.001f, 0.0f, 0.001f);
	}
}
