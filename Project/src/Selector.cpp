//All content � 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#include "Selector.h"
#include "BehaviorTree.h"
#include "Decorator.h"
#include "NodeBlackboard.h"

Selector::Selector( BehaviorTree *treeOwner ) : NodeBranch( treeOwner )
{

}

Selector::~Selector( )
{
}

int Selector::Update( float deltaTime )
{
    Node *nodeEval = NULL;
    int leafStatus = -1;
    m_status = NSTATUS_RUNNING;
    if ( m_currentNode == NULL )
    {
        m_currentNode = m_nodes.front( );
        m_currentNode->Start( );
    }
    else
    {
        nodeEval = m_currentNode;
        if ( m_currentNode->type == NTYPE_DECORATOR )
        {
            nodeEval = ((Decorator *)m_currentNode)->GetNodeDecorate( );
        }

        if ( nodeEval->type == NTYPE_BLACKBOARD )
        {
            ((NodeBlackboard *)nodeEval)->processValue( );
            leafStatus = NSTATUS_FAIL;
        }
        else
        {
            if ( nodeEval->type == NTYPE_BRANCH )
            {
                if ( nodeEval->GetStatus( ) == NSTATUS_WAIT || nodeEval->GetStatus( ) == NSTATUS_RUNNING )
                {
                    if ( m_reevaluateFromQuery == true )
                    {
                        ((NodeBranch *)nodeEval)->needParentReevaluation = true;
                    }
                    m_behaviorTree->UpdateBranchPointer( (NodeBranch *)m_currentNode );
                    m_status = NSTATUS_RUNNING;
                }
                else
                {
                    leafStatus = nodeEval->GetStatus( );
                    m_currentNode->Start( );
                }
            }
            else if ( nodeEval->type == NTYPE_QUERY && ((Query *)nodeEval)->GetReevaluate( ) == true && m_reevaluateFromQuery == false )
            {
                m_reevaluateFromQuery = true;
                m_queryNode = (Query *)m_currentNode;
                leafStatus = m_currentNode->Update( deltaTime );
            }
            else
            {
                leafStatus = m_currentNode->Update( deltaTime );
            }
        }

        switch ( leafStatus )
        {
        case NSTATUS_RUNNING:
            if ( m_reevaluateFromQuery == true && nodeEval->type != NTYPE_QUERY )
            {
                m_currentNode = m_queryNode;
            }
            if ( needParentReevaluation == true )
            {
                ((NodeBranch *)m_parent)->resetQuery( );
                m_behaviorTree->UpdateBranchPointer( m_parent->GetOverParent( ) );
            }
            m_status = NSTATUS_RUNNING;
            needParentReevaluation = false;
            break;
        case NSTATUS_FAIL:
            if ( needParentReevaluation == true )
            {
                ((NodeBranch *)m_parent)->resetQuery( );
                m_behaviorTree->UpdateBranchPointer( m_parent->GetOverParent( ) );
            }
            if ( m_reevaluateFromQuery == true && nodeEval->type != NTYPE_QUERY )
            {
                m_reevaluateFromQuery = false;
                m_queryNode = NULL;
            }
            for ( std::vector<Node*>::iterator it = m_nodes.begin( ); it != m_nodes.end( ); ++it )
            {
                if ( (*it) == m_currentNode )
                {
                    ++it;
                    if ( it == m_nodes.end( ) )
                    {
                        m_currentNode = NULL;
                        m_status = NSTATUS_FAIL;
                    }
                    else
                    {
                        m_currentNode = *it;
                        m_currentNode->Start( );
                        m_status = NSTATUS_RUNNING;
                    }
                    break;
                }
            }
            needParentReevaluation = false;
            break;
        case NSTATUS_SUCCESS:
            m_reevaluateFromQuery = false;
            m_queryNode = NULL;
            m_currentNode = NULL;
            m_status = NSTATUS_SUCCESS;
            needParentReevaluation = false;
            break;
        }
    }
    return m_status;
}

void Selector::onCollision( GameObject *other )
{
    if ( m_currentNode != NULL )
    {
        m_currentNode->onCollision( other );
    }
}

void Selector::onHit( )
{
    if ( m_currentNode != NULL )
    {
        m_currentNode->onHit( );
    }
}

void Selector::onPlayerDeath( )
{
    if ( m_currentNode != NULL )
    {
        m_currentNode->onPlayerDeath( );
    }
}