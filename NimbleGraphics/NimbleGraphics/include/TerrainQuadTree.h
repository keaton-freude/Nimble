#pragma once
#include <memory>
#include <vector>
#include <wrl/client.h>
#include "Structs.h"
#include "QuadTreeNode.h"
#include "Frustum.h"

using std::shared_ptr;
using std::unique_ptr;
using Microsoft::WRL::ComPtr;
using std::vector;

class TerrainQuadTree
{
public:
	TerrainQuadTree();
	TerrainQuadTree(const TerrainQuadTree &);
	~TerrainQuadTree();

	bool Initialize(ComPtr<ID3D11Device> device, int terrainVertexCount, shared_ptr<TerrainVertex> terrainVerts);
	void Draw(shared_ptr<Frustum> frustum, ComPtr<ID3D11DeviceContext> deviceContext);
	void CopyVertexArray(void* vertexList);
	int GetDrawCount();
	void PrintLeaves();
	int GetVisibleVertCount();
	void UpdateBuffers(ComPtr<ID3D11DeviceContext> deviceContext);
	void UpdateVerts(HeightMapData* data);

private:
	void CalculateMeshDimensions(int vertexCount, float& centerX, float& centerZ, float& meshWidth);
	void CreateTreeNode(shared_ptr<QuadTreeNode> node, float positionX, float positionZ, float width, ComPtr<ID3D11Device> device);
	int CountTriangles(float positionX, float positionZ, float width);
	bool IsTriangleContained(int index, float positionX, float positionZ, float width);
	void RenderNode(shared_ptr<QuadTreeNode> node, shared_ptr<Frustum> frustum, 
		ComPtr<ID3D11DeviceContext> deviceContext);
private:
	int _triangleCount;
	int _vertexCount;
	int _drawCount;
	const int MAX_TRIANGLES = 10000;
	shared_ptr<TerrainVertex> _vertexList;

	// STATS

	// how many verts are currently being rendered?
	int _visibleVertCount;

	// Head of the tree
	shared_ptr<QuadTreeNode> _parent;

	// Cached pointers to each of the leaf nodes
	vector<shared_ptr<QuadTreeNode>> _leaves;
};