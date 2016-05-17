//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#pragma once

#include "Node.h"
#include "Query.h"

class Decorator : public Node
{
public:
	Decorator(BehaviorTree *treeOwner, Node *nodeDecorate);
	~Decorator();

	void AddLeaf(Node *leaf);
	Node *GetNodeDecorate() { return m_nodeDecorate; }
	int Update(float deltaTime);
	int LeafType;
	void Start();
	operator Query* ();
    void onCollision( GameObject *other );
    void onHit(  );
protected:
	Node *m_nodeDecorate;
};