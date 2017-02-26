#pragma once
#include "Typedefs.h"
#include <vector>
#include "RenderObject.h"
#include "RayHit.h"

using std::vector;
/*
 * A render system will collect all objects to be drawn
 * and draw them to the screen. It will render RenderObjects
 * which are a Mesh + Material.
 */

class RenderSystem
{
public:
	RenderSystem();
	RenderSystem(D3DDevice device, D3DDeviceContext deviceContext);

	void AddObject(const RenderObject& renderObjectToAdd);
	void Update(MatrixRef viewMatrix, MatrixRef projectionMatrix);
	void Draw();
	RayHit CastRay(const Ray& ray);

private:
	D3DDevice _device;
	D3DDeviceContext _deviceContext;
	vector<RenderObject> _renderObjects;
};
