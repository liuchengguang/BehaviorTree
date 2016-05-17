//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#include "NodeBranch.h"

NodeBranch::NodeBranch( BehaviorTree *treeOwner ) : Node( treeOwner ), m_reevaluateFromQuery( false ), m_currentNode( NULL )
{
    m_nodes.clear( );
    m_status = NSTATUS_WAIT;
    type = NTYPE_BRANCH;
}

NodeBranch::~NodeBranch( )
{
    for each (Node *node in m_nodes)
    {
        delete node;
    }
}

void NodeBranch::Start( )
{
    m_reevaluateFromQuery = false;
    m_status = NSTATUS_WAIT;
    m_currentNode = NULL;
}

void NodeBranch::addLeaf( Node *leaf )
{
    leaf->SetParent( this );
    leaf->SetOverParent( this );
    m_nodes.push_back( leaf );
}

void NodeBranch::resetQuery( )
{
    m_currentNode = m_queryNode;
}

Node *NodeBranch::GetCurrentNode( )
{ 
    return m_currentNode;
}