#pragma once
#include "Singleton.h"
#include "LineMesh.h"

class DebugLineManager: public Singleton<DebugLineManager>
{
public:
	
	void Load(ComPtr<ID3D11Device> device)
	{
		_device = device;
	}

	// direction is normalized, so we just draw from origin to direction
	void AddNormal(Vector3 origin, Vector3 direction)
	{
		_lines.emplace_back(_device, origin, origin + direction);
	}

	vector<LineMesh>& GetLines()
	{
		return _lines;
	}

	void Clear()
	{
		_lines.clear();
	}

private:
	vector<LineMesh> _lines;
	ComPtr<ID3D11Device> _device;
};
