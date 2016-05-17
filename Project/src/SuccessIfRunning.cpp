//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#include "SuccessIfRunning.h"

SuccessIfRunning::SuccessIfRunning( BehaviorTree *treeOwner, Node *nodeDecorate ) : Decorator( treeOwner, nodeDecorate )
{

}

SuccessIfRunning::~SuccessIfRunning( )
{

}

int SuccessIfRunning::Update( float deltaTime )
{
    int taskStatus = Decorator::Update( deltaTime );
    if ( taskStatus == NSTATUS_RUNNING )
    {
        taskStatus = NSTATUS_SUCCESS;
    }
    return taskStatus;
}