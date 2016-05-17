//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#pragma once

#include "Node.h"
#include "Query.h"
#include <vector>

class NodeBranch : public Node
{
public:
    NodeBranch( BehaviorTree *treeOwner );
    ~NodeBranch( );

    void resetQuery( );
    void addLeaf( Node *leaf );
    void Start( );
    Node *GetCurrentNode( );

    bool needParentReevaluation;
protected:
    Node *m_currentNode;
    std::vector<Node *> m_nodes;
    bool m_reevaluateFromQuery;
    Query *m_queryNode;
private:
};