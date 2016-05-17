//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#include "Node.h"
#include "BehaviorTree.h"

Node::Node( BehaviorTree *treeOwner ) : m_behaviorTree( treeOwner ), m_blackboard( treeOwner->getBlackboard( ) ), m_parent( NULL ), m_status( NSTATUS_WAIT ), type( NTYPE_TASK ), m_overParent( this )
{

}

Node::~Node( )
{

}

void Node::SetParent( Node *parent )
{
    m_parent = parent;
}

void Node::SetOverParent( Node *overParent )
{
    m_overParent = overParent;
}

int Node::GetStatus( )
{
    return m_status;
}

void Node::onCollision( GameObject *other )
{

}

void Node::onHit(  )
{

}

void Node::onPlayerDeath( )
{

}