#include "DebugLineManager.h"

void DebugLineManager::Load(ComPtr<ID3D11Device> device)
{
	_device = device;
}

void DebugLineManager::AddNormal(Vector3 origin, Vector3 direction)
{
	_lines.emplace_back(_device, origin, origin + direction);
}

vector<LineMesh>& DebugLineManager::GetLines()
{
	return _lines;
}

void DebugLineManager::Clear()
{
	_lines.clear();
}
