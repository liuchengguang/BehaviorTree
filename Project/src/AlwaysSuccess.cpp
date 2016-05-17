//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#include "AlwaysSuccess.h"

AlwaysSuccess::AlwaysSuccess( BehaviorTree *treeOwner, Node *nodeDecorate ) : Decorator( treeOwner, nodeDecorate )
{

}

AlwaysSuccess::~AlwaysSuccess( )
{

}

int AlwaysSuccess::Update( float deltaTime )
{
    return NSTATUS_SUCCESS;
}