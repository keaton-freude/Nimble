#pragma once

#include <list>
#include <memory>

using std::list;
using std::shared_ptr;

class SceneNode
{
public:
	// constructor
	SceneNode()
	{
		
	}

	// destructor - calls destroy
	virtual ~SceneNode()
	{
		Destroy();
	}

	// release this object from memory
	void Release() const
	{
		delete this;
	}

	// update our scene node
	virtual void Update()
	{
		// loop through the list and update the children
		for( auto child : m_lstChildren )
		{
			child->Update();
		}
	}

	// destroy all the children
	void Destroy()
	{
		for (auto child : m_lstChildren)
		{
			child->Release();
		}

		m_lstChildren.clear();
	}

	// add a child to our custody
	void AddChild(shared_ptr<SceneNode> pNode)
	{
		m_lstChildren.push_back(pNode);
	}

protected:
	// list of children
	list<shared_ptr<SceneNode>> m_lstChildren;
};

class GeometryNode : public SceneNode
{
public:
	GeometryNode()
	{
	}

	~GeometryNode()
	{
	}

	void Update()
	{
		// Draw our geometry here!

		SceneNode::Update();
	}
};

class DOFNode : public SceneNode
{
public:
	DOFNode()
	{
		
	}

	~DOFNode()
	{
		
	}

	void Initialize(float m[4][4])
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				m_fvMatrix[i][j] = m[i][j];
	}

	void Update()
	{
		//glPushMatrix();
		//glLoadMatrix((float*)m_fvMatrix);

		SceneNode::Update();

		//glPopMatrix();
	}

private:
	float m_fvMatrix[4][4];
};

class OctNode : public SceneNode
{
public:
	OctNode();
	~OctNode();
	void Partition();
private:

};

/* The purpose of a SceneGraph is mostly to support an easy way to apply affine transformations to groups of objects.
 * It must also support an Octree heirarchy to spatially partition the SceneGraph for ray intersection tests.
 * The leaf nodes of this heirarchy are always bounding boxs around non-moving meshs in the scene.*/
class SceneGraph
{
	friend SceneNode;

public:
	SceneGraph();
	~SceneGraph();

private:

};