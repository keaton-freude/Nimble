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
	Heightmap(unsigned int width, unsigned int height);
	Heightmap(ComPtr<ID3D11Device> device, string heightMapPath);

	void Add(Vector3 location, float radius, float min, float max);
	void Save();

	void LoadFromFile(ComPtr<ID3D11Device> device, string heightMapPath);
	shared_ptr<HeightMapData> GetHeightMapData();
	unsigned int GetWidth();
	unsigned int GetHeight();
private:
	bool CalculateNormals();

private:
	vector<double> _splineX;
	vector<double> _splineY;
	bitmap_image _image;	
	tk::spline _spline;
	shared_ptr<HeightMapData> _heightMap; // indexed by x,y
};