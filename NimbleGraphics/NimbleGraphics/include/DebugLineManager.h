#pragma once
#include "Singleton.h"
#include "LineDrawable.h"

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
		auto line = LineDrawable();
		line.SetLine(_device, origin, origin + direction);
		_lines.push_back(line);
	}

	vector<LineDrawable>& GetLines()
	{
		return _lines;
	}

	void Clear()
	{
		_lines.clear();
	}

private:
	vector<LineDrawable> _lines;
	ComPtr<ID3D11Device> _device;
};
