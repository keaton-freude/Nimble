#pragma once

#include <memory>
#include <SimpleMath.h>

#include "Transform.h"

using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Ray;
using std::shared_ptr;


class Camera
{
public:
	Camera();
	Camera(const Camera&);
	Camera(Vector3 initial_position);
	Camera(Vector3 initial_position, Vector3 initial_rotation);
	~Camera();
	

	inline Vector3 GetPosition() const { return _transform._position; }
	inline Vector3 GetRotation() const { return _transform._rotation; }
	inline shared_ptr<Matrix> GetViewMatrix() { return _viewMatrix; }

	void SetPosition(float x, float y, float z);
	void SetPosition(Vector3 new_position);
	void Translate(Vector3 offset);
	void Translate(const float& x, const float& y, const float& z);
	void Rotate(Vector3 delta_rotation);
	void TranslateRotated(Vector3 offset);
	void Update();
	Ray GetMouseRay(Vector2 mousePosition, Matrix projectionMatrix);
private:
	void SetSafePosition();

private:
	Transform _transform;
	shared_ptr<Matrix> _viewMatrix;
};