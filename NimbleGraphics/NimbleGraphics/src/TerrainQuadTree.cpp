#include "TerrainQuadTree.h"
#include "Logger.h"


void TerrainQuadTree::CopyVertexArray(void* vertexList)
{
	memcpy(_vertexList.get(), vertexList, sizeof(TerrainVertex) * _vertexCount);
}

TerrainQuadTree::TerrainQuadTree()
{
	LOG_INFO("Quad Tree Constructed.");
	_leaves = vector<shared_ptr<QuadTreeNode>>();
}

TerrainQuadTree::TerrainQuadTree(const TerrainQuadTree &)
{
}

TerrainQuadTree::~TerrainQuadTree()
{
	LOG_INFO("Quad Tree Destruct!");
}

bool TerrainQuadTree::Initialize(ComPtr<ID3D11Device> device, int terrainVertexCount, shared_ptr<TerrainVertex> terrainVerts)
{
	float centerX, centerZ, width;

	_vertexCount = terrainVertexCount;

	_triangleCount = _vertexCount / 3;

	_vertexList.reset(new TerrainVertex[_vertexCount]);

	CopyVertexArray((void*)terrainVerts.get());

	//CalculateMeshDimensions(_vertexCount, centerX, centerZ, width);

	centerX = 128.0f;
	centerZ = 128.0f;
	width = 256.0f;

	_parent = make_shared<QuadTreeNode>();

	CreateTreeNode(_parent, centerX, centerZ, width, device);
	
	//_vertexList.reset();

	PrintLeaves();

	return true;
}

void TerrainQuadTree::Draw(shared_ptr<Frustum> frustum, ComPtr<ID3D11DeviceContext> deviceContext)
{
	_drawCount = 0;
	_visibleVertCount = 0;

	RenderNode(_parent, frustum, deviceContext);
}

int TerrainQuadTree::GetDrawCount()
{
	return _drawCount;
}

void TerrainQuadTree::PrintLeaves()
{
	for (auto &element : _leaves)
	{
		std::string message = "Leaf: (X,Z,width) - (" + std::to_string(element->positionX)
			+ ", " + std::to_string(element->positionZ) + ", " + std::to_string(element->width)
			+ ")";
		LOG_INFO(message);
	}
}

int TerrainQuadTree::GetVisibleVertCount()
{
	return _visibleVertCount;
}

void TerrainQuadTree::UpdateBuffers(ComPtr<ID3D11DeviceContext> deviceContext)
{
	// for each leaf node, tell that node it needs to update its buffers
	for (auto &element : _leaves)
	{
		element->Update(deviceContext, this->_vertexList);
	}
}

void TerrainQuadTree::UpdateVerts(HeightMapData * data)
{
	int terrain_index = 0;
	int heightmap_index = 0;

	for (int i = 0; i < _vertexCount; ++i)
	{
		_vertexList.get()[i].position.y = data[i].y;
	}
}

void TerrainQuadTree::CalculateMeshDimensions(int vertexCount, float & centerX, float & centerZ, 
	float & meshWidth)
{
	int i;
	float maxWidth, maxDepth, minWidth, minDepth, width, depth, maxX, maxZ;

	centerX = 0.0f;
	centerZ = 0.0f;

	for (i = 0; i < vertexCount; ++i)
	{
		centerX += _vertexList.get()[i].position.x;
		centerZ += _vertexList.get()[i].position.z;
	}

	centerX = centerX / (float)vertexCount;
	centerZ = centerZ / (float)vertexCount;

	maxWidth = 0.0f;
	maxDepth = 0.0f;

	minWidth = fabsf(_vertexList.get()[0].position.x - centerX);
	minDepth = fabsf(_vertexList.get()[0].position.z - centerZ);

	for (i = 0; i < vertexCount; ++i)
	{
		width = fabsf(_vertexList.get()[i].position.x - centerX);
		depth = fabsf(_vertexList.get()[i].position.z - centerZ);

		if (width < maxWidth)
			maxWidth = width;
		if (depth > maxDepth)
			maxDepth = depth;
		if (width < minWidth)
			minWidth = width;
		if (depth < minDepth)
			minDepth = depth;
	}

	maxX = (float)max(fabs(minWidth), fabs(maxWidth));
	maxZ = (float)max(fabs(minDepth), fabs(maxDepth));

	meshWidth = max(maxX, maxZ) * 2.0f;
}

void TerrainQuadTree::CreateTreeNode(shared_ptr<QuadTreeNode> node, float positionX, float positionZ, 
	float width, ComPtr<ID3D11Device> device)
{
	int numTriangles, i, count, vertexCount, index, vertexIndex;
	float offsetX, offsetZ;
	shared_ptr<TerrainVertex> vertices;
	shared_ptr<unsigned long> indices;
	bool result;

	node->positionX = positionX;
	node->positionZ = positionZ;
	node->width = width;

	node->triangleCount = 0;

	node->children[0] = 0;
	node->children[1] = 0;
	node->children[2] = 0;
	node->children[3] = 0;
	
	numTriangles = CountTriangles(positionX, positionZ, width);

	// Case 1
	if (numTriangles == 0)
		return;

	// Case 2 - We need to split this up into 4 quads
	if (numTriangles > MAX_TRIANGLES)
	{
		for (i = 0; i < 4; ++i)
		{
			offsetX = (((i % 2) < 1) ? -1.0f : 1.0f) * (width / 4.0f);
			offsetZ = (((i % 4) < 2) ? -1.0f : 1.0f) * (width / 4.0f);

			count = CountTriangles((positionX + offsetX), (positionZ + offsetZ), (width / 2.0f));
			if (count > 0)
			{
				node->children[i] = make_shared<QuadTreeNode>();

				CreateTreeNode(node->children[i], (positionX + offsetX), (positionZ + offsetZ), 
					(width / 2.0f), device);
			}
		}

		return;
	}
	
	// Case 3
	// If we've gotten this far then we need to create the vertexBuffer for the ndoe
	// we are a leaf node
	node->triangleCount = numTriangles;
	vertexCount = numTriangles * 3;

	vertices = shared_ptr<TerrainVertex>(new TerrainVertex[vertexCount]);
	indices = shared_ptr<unsigned long>(new unsigned long[vertexCount]);

	index = 0;

	for (i = 0; i < _triangleCount; ++i)
	{
		result = IsTriangleContained(i, positionX, positionZ, width);
		if (result)
		{
			vertexIndex = i * 3;

			vertices.get()[index].position = _vertexList.get()[vertexIndex].position;
			vertices.get()[index].texture = _vertexList.get()[vertexIndex].texture;
			vertices.get()[index].normal = _vertexList.get()[vertexIndex].normal;
			indices.get()[index] = index;
			index++;

			vertexIndex++;
			vertices.get()[index].position = _vertexList.get()[vertexIndex].position;
			vertices.get()[index].texture = _vertexList.get()[vertexIndex].texture;
			vertices.get()[index].normal = _vertexList.get()[vertexIndex].normal;
			indices.get()[index] = index;
			index++;

			vertexIndex++;
			vertices.get()[index].position = _vertexList.get()[vertexIndex].position;
			vertices.get()[index].texture = _vertexList.get()[vertexIndex].texture;
			vertices.get()[index].normal = _vertexList.get()[vertexIndex].normal;
			indices.get()[index] = index;
			index++;
		}
	}

	node->data->SetVertexBuffer(VertexBuffer::CreateTerrainNode(device, vertices, vertexCount, indices));
	_leaves.push_back(node);
}

int TerrainQuadTree::CountTriangles(float positionX, float positionZ, float width)
{
	int count, i;
	bool result;

	// Initialize the count to zero.
	count = 0;

	// Go through all the triangles in the entire mesh and check which ones should be inside this node.
	for (i = 0; i < _triangleCount; i++)
	{
		// If the triangle is inside the node then increment the count by one.
		result = IsTriangleContained(i, positionX, positionZ, width);
		if (result == true)
		{
			count++;
		}
	}

	return count;
}

bool TerrainQuadTree::IsTriangleContained(int index, float positionX, float positionZ, float width)
{
	float radius;
	int vertexIndex;
	float x1, z1, x2, z2, x3, z3;
	float minimumX, maximumX, minimumZ, maximumZ;


	// Calculate the radius of this node.
	radius = width / 2.0f;

	// Get the index into the vertex list.
	vertexIndex = index * 3;

	// Get the three vertices of this triangle from the vertex list.
	x1 = _vertexList.get()[vertexIndex].position.x;
	z1 = _vertexList.get()[vertexIndex].position.z;
	vertexIndex++;

	x2 = _vertexList.get()[vertexIndex].position.x;
	z2 = _vertexList.get()[vertexIndex].position.z;
	vertexIndex++;

	x3 = _vertexList.get()[vertexIndex].position.x;
	z3 = _vertexList.get()[vertexIndex].position.z;

	// Check to see if the minimum of the x coordinates of the triangle is inside the node.
	minimumX = min(x1, min(x2, x3));
	if (minimumX > (positionX + radius))
	{
		return false;
	}

	// Check to see if the maximum of the x coordinates of the triangle is inside the node.
	maximumX = max(x1, max(x2, x3));
	if (maximumX < (positionX - radius))
	{
		return false;
	}

	// Check to see if the minimum of the z coordinates of the triangle is inside the node.
	minimumZ = min(z1, min(z2, z3));
	if (minimumZ >(positionZ + radius))
	{
		return false;
	}

	// Check to see if the maximum of the z coordinates of the triangle is inside the node.
	maximumZ = max(z1, max(z2, z3));
	if (maximumZ < (positionZ - radius))
	{
		return false;
	}

	return true;
}

void TerrainQuadTree::RenderNode(shared_ptr<QuadTreeNode> node, shared_ptr<Frustum> frustum, 
	ComPtr<ID3D11DeviceContext> deviceContext)
{
	int count, i, indexCount;
	bool result;

	result = frustum->CheckCube(node->positionX, 0.0f, node->positionZ, 
		node->width / 2.0f);

	if (!result)
		return;

	count = 0;
	for (i = 0; i < 4; ++i)
	{
		if (node->children[i] != 0)
		{
			++count;
			RenderNode(node->children[i], frustum, deviceContext);
		}
	}

	if (count != 0)
		return;

	_drawCount++;
	_visibleVertCount += (node->triangleCount * 3);
	node->data->Draw(deviceContext, node->triangleCount * 3);
}
