//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#pragma once
#include "Debug.h"
#include "Decorator.h"

class SuccessIfRunning : public Decorator
{
public:
    SuccessIfRunning( BehaviorTree *treeOwner, Node *nodeDecorate );
    ~SuccessIfRunning( );

    int Update( float deltaTime );
private:
    Node *m_task;
};