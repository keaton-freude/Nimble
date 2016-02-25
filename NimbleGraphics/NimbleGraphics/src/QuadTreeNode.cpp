#include "..\include\QuadTreeNode.h"

QuadTreeNode::QuadTreeNode()
	: data(new TerrainDrawable()), width(0.0f), positionX(0.0f), positionZ(0.0f), triangleCount(0)
{
	children[0] = 0;
	children[1] = 0;
	children[2] = 0;
	children[3] = 0;
}

QuadTreeNode::QuadTreeNode(float positionX, float positionY, float width)
	: data(new TerrainDrawable()), width(width), positionX(positionX), positionZ(positionY), triangleCount(0)
{
	children[0] = 0;
	children[1] = 0;
	children[2] = 0;
	children[3] = 0;
}

QuadTreeNode::QuadTreeNode(const QuadTreeNode &)
{
}

QuadTreeNode::~QuadTreeNode()
{
	LOG_INFO("Quad Tree Node destruct!");
}

void QuadTreeNode::Update(ComPtr<ID3D11DeviceContext> deviceContext, shared_ptr<TerrainVertex> vertices)
{
	// our positionX and positionZ refer to the MIDDLE of the quad, offset to the top-left corner
	// so the array copys are easy
	int start_x = positionX - (width / 2);
	int start_z = positionZ - (width / 2);
	
	data->Update(deviceContext, start_x, start_z, width, vertices);
}