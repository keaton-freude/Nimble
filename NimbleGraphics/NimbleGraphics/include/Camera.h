#pragma once
#include "Typedefs.h"
#include <memory>

#include "Transform.h"

using std::shared_ptr;


class Camera
{
public:
	Camera();
	explicit Camera(Vector3 initial_position);
	Camera(Vector3 initial_position, Vector3 initial_rotation);
	~Camera();

	const Vector3& GetPosition() const;
	const Vector3& GetRotation() const;
	const Matrix& GetViewMatrix() const;

	void SetPosition(float x, float y, float z);
	void SetPosition(Vector3 new_position);
	void Translate(Vector3 offset);
	void Translate(const float& x, const float& y, const float& z);
	void Rotate(Vector3 delta_rotation);
	void Update();

	DirectX::SimpleMath::Ray GetMouseRay(DirectX::SimpleMath::Vector2 mousePosition, Matrix projectionMatrix) const;
private:
	void SetSafePosition();

private:
	Transform _transform;
	Matrix _viewMatrix;
};