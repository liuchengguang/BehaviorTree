//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#include "Task.h"

Task::Task( BehaviorTree *treeOwner ) : Node( treeOwner )
{
    type = NTYPE_TASK;
}

Task::~Task( )
{

}

void Task::Start( )
{
    m_status = NSTATUS_WAIT;
}