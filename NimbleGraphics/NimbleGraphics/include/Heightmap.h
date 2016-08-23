#pragma once
#include <d3d11.h>
#include <memory>
#include <vector>
#include <math.h>
#include <wrl\client.h>
#include "..\..\..\Externals\Bitmap\bitmap_image.hpp"
#include "spline.h"
#include "Structs.h"

using Microsoft::WRL::ComPtr;
using std::string;
using std::shared_ptr;
using DirectX::SimpleMath::Vector3;
using std::vector;

class Heightmap
{
public:
	Heightmap();
	~Heightmap();
	Heightmap(unsigned int width, unsigned int height, float resolution=1.0f);
	Heightmap(ComPtr<ID3D11Device> device, string heightMapPath);

	void Add(Vector3 location, float radius, float min, float max);
	
	void SmoothAdd(Vector3 location, float radius, float intensity);
	void Save();

	void LoadFromFile(ComPtr<ID3D11Device> device, string heightMapPath);
	shared_ptr<TerrainVertex> GetHeightMapData();
	unsigned int GetWidth();
	unsigned int GetHeight();

	float GetHighestPoint()
	{
		return highest_point;
	}
	shared_ptr<TerrainVertex> _heightMap; // indexed by x,y

private:
	bool CalculateNormals();
	float highest_point;
	void CalculateHighestPoint();


private:
	vector<double> _splineX;
	vector<double> _splineY;
	bitmap_image _image;	
	tk::spline _spline;

};