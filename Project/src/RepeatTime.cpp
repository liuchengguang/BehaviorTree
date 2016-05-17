//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#include "RepeatTime.h"

RepeatTime::RepeatTime( BehaviorTree *treeOwner, Node *nodeDecorate ) : Decorator( treeOwner, nodeDecorate )
{

}

RepeatTime::~RepeatTime( )
{

}

void RepeatTime::Start( )
{
    Decorator::Start( );
    m_timeAcumulator = 0;
    m_timeValue = m_blackboard->GetParameterFloat( "REPT_val" );
}

int RepeatTime::Update( float deltaTime )
{
    m_timeAcumulator += deltaTime;
    int taskStatus = Decorator::Update( deltaTime );
    switch ( taskStatus )
    {
    case NSTATUS_SUCCESS:
    case NSTATUS_FAIL:
        if ( m_timeAcumulator  > m_timeValue )
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