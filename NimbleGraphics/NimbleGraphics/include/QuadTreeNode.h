#pragma once
#include <memory>
#include "TerrainDrawable.h"
#include "Logger.h"

using namespace std;


class QuadTreeNode
{
public:
	// pointer to our data, in this case a TerrainDrawable which will draw a chunk of terrain
	// this pointer is only valid when this node is a leaf (meets some criteria of the tree)
	shared_ptr<TerrainDrawable> data;

	// pointer to the children of this node. Because we are partitioning based on a quad
	// we have 4 children which are subdivisions of ourself
	shared_ptr<QuadTreeNode> children[4];

	// How wide is our Quad?
	float width;

	float positionX;
	float positionZ;

	// How many triangles are in our Quad?
	int triangleCount;

	QuadTreeNode();
	QuadTreeNode(float positionX, float positionY, float width);
	QuadTreeNode(const QuadTreeNode &);
	~QuadTreeNode();

	void Update(ComPtr<ID3D11DeviceContext> deviceContext, shared_ptr<TerrainVertex> vertices);
	

private:
};