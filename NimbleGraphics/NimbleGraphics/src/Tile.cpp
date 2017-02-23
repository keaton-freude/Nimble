//#include "Tile.h"
//
//Tile::Tile()
//{
//}
//
//Tile::Tile(D3DDevice device, unsigned width, unsigned height, float resolution)
//	: _heightmap(width, height, resolution)
//{
//	// Create the mesh
//	auto vertices = vector<TerrainVertex>(width * height);
//
//	for(auto i = 0; i < width; ++i)
//	{
//		for(auto j = 0; j < height; ++j)
//		{
//			auto index = i * width + j;
//			vertices[index] = TerrainVertex(Vector3(i, 0, j), Vector2::Zero, Vector3::UnitY);
//		}
//	}
//
//	auto indicies = vector<unsigned long>(width * height * 6);
//	auto index = 0;
//
//	for (auto j = 0; j < height; ++j)
//	{
//		for (auto i = 0; i < width; ++i)
//		{
//			// go in groups of 3
//			// upper-left
//			indicies[index++] = ((j + 1) * height) + i;
//			// upper-right
//			indicies[index++] = ((j + 1) * height) + (i + 1);
//			// bottom-left
//			indicies[index++] = j * height + i;
//
//			// bottom-left
//			indicies[index++] = j * height + i;
//			// upper-right
//			indicies[index++] = ((j + 1) * height) + (i + 1);
//			// bottom-right
//			indicies[index++] = j * height + (i + 1);
//		}
//	}
//
//	_mesh = TerrainMesh(device, vertices, indicies);
//}
//
//Tile::~Tile()
//{
//}
//
//void Tile::Draw()
//{
//
//}
