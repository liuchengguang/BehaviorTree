//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#include "Decorator.h"
#include "NodeBranch.h"

Decorator::Decorator( BehaviorTree *treeOwner, Node *nodeDecorate ) : Node( treeOwner ), m_nodeDecorate( nodeDecorate )
{
    type = NTYPE_DECORATOR;
    LeafType = nodeDecorate->type;
    m_nodeDecorate->SetOverParent( this );
    m_nodeDecorate->SetParent( m_parent );
}

Decorator::~Decorator( )
{
    delete m_nodeDecorate;
}

void Decorator::Start( )
{
    m_nodeDecorate->Start( );
}

int Decorator::Update( float deltaTime )
{
    return m_nodeDecorate->Update( deltaTime );
}

void Decorator::AddLeaf( Node *leaf )
{
    if ( m_nodeDecorate->type == NTYPE_BRANCH )
    {
        ((NodeBranch*)m_nodeDecorate)->addLeaf( leaf );
    }
}

void Decorator::onCollision( GameObject *other )
{
    m_nodeDecorate->onCollision( other );
}

void Decorator::onHit(  )
{
    m_nodeDecorate->onHit(  );
}