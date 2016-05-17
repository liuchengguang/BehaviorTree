//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#include "Inverter.h"

Inverter::Inverter( BehaviorTree *treeOwner, Node *nodeDecorate ) : Decorator( treeOwner, nodeDecorate )
{

}

Inverter::~Inverter( )
{

}

int Inverter::Update( float deltaTime )
{
    int status = Decorator::Update( deltaTime );
    switch ( status )
    {
    case NSTATUS_WAIT:
    case NSTATUS_RUNNING:
        break;
    case NSTATUS_SUCCESS:
        status = NSTATUS_FAIL;
        break;
    case NSTATUS_FAIL:
        status = NSTATUS_SUCCESS;
        break;
    }
    m_status = status;
    return status;
}