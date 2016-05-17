//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#include "AlwaysFail.h"

AlwaysFail::AlwaysFail( BehaviorTree *treeOwner, Node *nodeDecorate ) : Decorator( treeOwner, nodeDecorate )
{

}

AlwaysFail::~AlwaysFail( )
{

}

int AlwaysFail::Update( float deltaTime )
{
    return NSTATUS_FAIL;
}