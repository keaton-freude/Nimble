#pragma once
#include "Singleton.h"
#include "LineMesh.h"

class DebugLineManager: public Singleton<DebugLineManager>
{
public:

	void Load(ComPtr<ID3D11Device> device);

	// direction is normalized, so we just draw from origin to direction
	void AddNormal(Vector3 origin, Vector3 direction);

	vector<LineMesh>& GetLines();

	void Clear();

private:
	vector<LineMesh> _lines;
	ComPtr<ID3D11Device> _device;
};
