//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#include "Repeater.h"

Repeater::Repeater( BehaviorTree *treeOwner, Node *nodeDecorate ) : Decorator( treeOwner, nodeDecorate )
{

}

Repeater::~Repeater( )
{

}

void Repeater::Start( )
{
    Decorator::Start( );
    m_timesAcumulator = 0;
    m_timesValue = m_blackboard->GetParameterFloat( "REP_val" );
}

int Repeater::Update( float deltaTime )
{
    int taskStatus = Decorator::Update( deltaTime );
    switch ( taskStatus )
    {
    case NSTATUS_SUCCESS:
    case NSTATUS_FAIL:
        m_timesAcumulator += 1;
        if ( m_timesAcumulator  < m_timesValue )
        {
            m_nodeDecorate->Start( );
            return NSTATUS_RUNNING;
        }
        else
        {
            return taskStatus;
        }
        break;
    case NSTATUS_RUNNING:
        return taskStatus;
        break;
    }
    return NSTATUS_RUNNING;
}