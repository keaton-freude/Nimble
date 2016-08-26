#pragma once

#include <memory>
#include <SimpleMath.h>

#include "Transform.h"
#include "Logger.h"

using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Ray;
using std::shared_ptr;


class Camera
{
public:
	Camera(): _transform(Vector3(0.001f, 0.001f, 0.001f), Vector3(0.0f, 0.0f, 0.0f), 
		Vector3(1.0f, 1.0f, 1.0f)), _viewMatrix(new Matrix())
	{	
	}

	explicit Camera(Vector3 initial_position) : _transform(initial_position, Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.0f, 0.0f, 0.0f)), _viewMatrix(new Matrix())
	{
		// in case we are in an unsafe location
		this->SetSafePosition();
	}

	Camera(Vector3 initial_position, Vector3 initial_rotation): _transform(initial_position, initial_rotation,
		Vector3(0.0f, 0.0f, 0.0f)), _viewMatrix(new Matrix())
	{
		this->SetSafePosition();
	}

	~Camera()
	{
		LOG_INFO("Camera destruct!");
	}

	Vector3 GetPosition() const { return _transform._position; }
	Vector3 GetRotation() const { return _transform._rotation; }
	shared_ptr<Matrix> GetViewMatrix() const { return _viewMatrix; }

	void SetPosition(float x, float y, float z)
	{
		_transform._position = Vector3(x, y, z);
		this->SetSafePosition();
	}

	void SetPosition(Vector3 new_position)
	{
		_transform._position = new_position;
		this->SetSafePosition();
	}

	void Translate(Vector3 offset)
	{
		_transform._position += offset;
		this->SetSafePosition();
	}

	void Translate(const float& x, const float& y, const float& z)
	{
		this->Translate(Vector3(x, y, z));
	}

	void Rotate(Vector3 delta_rotation)
	{
		_transform._rotation += delta_rotation;
	}

	void TranslateRotated(Vector3 offset)
	{
		
	}

	void Update() const
	{
		auto rotationMatrixX = Matrix::CreateRotationX(_transform._rotation.x);
		auto rotationMatrixY = Matrix::CreateRotationY(_transform._rotation.y);

		auto transformedReference = Vector3::Transform(Vector3(0.0f, 0.0f, 1.0f), rotationMatrixX);
		transformedReference = Vector3::Transform(transformedReference, rotationMatrixY);

		auto cameraLookAt = _transform._position + transformedReference;

		*_viewMatrix = XMMatrixLookAtLH(_transform._position, cameraLookAt, Vector3::UnitY);
	}

	Ray GetMouseRay(Vector2 mousePosition, Matrix projectionMatrix) const
	{
		auto pointX = mousePosition.x;
		auto pointY = mousePosition.y;

		pointX = pointX / projectionMatrix._11;
		pointY = pointY / projectionMatrix._22;

		auto matView = *_viewMatrix;
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

		return Ray(rayOrig, direction);
	}
private:
	void SetSafePosition()
	{
		if (_transform._position == Vector3::Zero)
		{
			// This causes divide by zero in XMMatrixLookAtLH, offset by small amount
			_transform._position += Vector3(0.001f, 0.0f, 0.001f);
		}
	}

private:
	Transform _transform;
	shared_ptr<Matrix> _viewMatrix;
};